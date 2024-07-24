/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 04:01:04 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/24 03:52:01 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_status;

void	print_args(t_cmd *lst)
{
	int	x = 0;

	while (lst)
	{
		printf("lst %d :\n", x);
		int	i = 0;
		while (lst->argv[i])
		{
			printf("lst->argv[%d]: %s\n", i, lst->argv[i]);
			i++;
		}
		lst = lst->next;
		x++;
	}
	printf("\n");
}

void	free_cmd_lst(t_cmd *lst)
{
	t_cmd	*current;
	t_cmd	*next;

	current = lst;
	while (current)
	{
		next = current->next;
		free(current->cmd);
		free_all(current->argv);
		free(current);
		current = next;
	}
}

void    handling_shlvl(t_list *list)
{
    char    *shl_lvl;
    char    *lvl;

    shl_lvl = my_getenv("SHLVL", list);
    if (!shl_lvl)
        update_env("SHLVL", "1", list);
    else if (ft_atoi(shl_lvl) > 999)
    {
        printf("Minishell: warning: shell level ");
        printf("(%d) too high, resetting to 1\n", ft_atoi(shl_lvl));
        update_env("SHLVL", "1", list);
    }
    else if (ft_atoi(shl_lvl) < 0)
        update_env("SHLVL", "0", list);
    else if (ft_atoi(shl_lvl) == 999)
        update_env("SHLVL", "", list);
    else
    {
        lvl = ft_itoa(ft_atoi(shl_lvl) + 1);
        if(!lvl)
            exit(EXIT_FAILURE);
        add_the_value("SHLVL", lvl, list);
        free(lvl);
    }
}

void	free_list(t_list *list)
{
	t_env	*current;
	t_env	*next;

	current = list->envs;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	free(list);
}

int	main(int ac, char **av, char **env)
{
	char			*temp;
	t_cmd			*lst;
	t_list			*list;
	char			*str;
	char			**res;
	struct termios	copy;

	(void)av;
	g_signal_status = 0;
	lst = malloc(sizeof(t_cmd));
	list = malloc(sizeof(t_list));
	if (ac != 1 || !lst || !list)
		return (1);
	list->envs = env_init(env);
	handling_shlvl(list);
	while (1)
	{
		rl_catch_signals = 0;
		check_signals();
		temp = readline("Minishell-$ ");
		if (!temp || !isatty(0))
		{
			printf("exit\n");
			break ;
		}
		add_history(temp);
		if (!ft_strlen(temp) || is_blank(temp))
		{
			ex_st(0, 1);
			free(temp);
			continue ;
		}
		str = add_space(temp);
		if (syn_error(str))
		{
			free(temp), free(str), ex_st(258, 1);
			continue ;
		}
		if (!str)
			continue ;
		change_to_garb(str);
		if (handle_single_double(str))
			continue ;
		res = ft_split(str, '|');
		if (!res)
			continue ;
		lst = build_arr(res);
		if (!lst)
			continue ;
		back_to_ascii(lst);
		if (is_heredoc(lst))
			heredoc(lst, list);
		expand(lst, list);
		remove_qoutes(&lst);
		g_signal_status = 1;
		tcgetattr(0, &copy);
		execution(lst, list);
		tcsetattr(0, 0, &copy);
		g_signal_status = 0;
		free_cmd_lst(lst);
		free(str);
		free(temp);
		free_all(res);
	}
	free_list(list);
}
