/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 04:01:04 by mkimdil           #+#    #+#             */
/*   Updated: 2024/06/02 21:56:39 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_qoutes(t_cmd **lst)
{
	t_cmd	*current;
	char	**argv;
	char	*arg;
	int		len;
	int		i;
	int		j;

	current = *lst;
	while (current != NULL)
	{
		argv = current->argv;
		while (*argv != NULL)
		{
			arg = *argv;
			len = ft_strlen(arg);
			i = 0;
			j = 0;
			while (i < len)
			{
				if (arg[i] != '"' && arg[i] != '\'')
					arg[j++] = arg[i];
				i++;
			}
			arg[j] = '\0';
			argv++;
		}
		current = current->next;
	}
}

void	function_sigint(int sig)
{
	if (sig == SIGINT && g_signal_status == 0)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGINT && g_signal_status == 1)
		write(1, "\n", 1);
}

void	function_sigwuit(int sig)
{
	if (sig == SIGQUIT && g_signal_status ==1)
		write(1, "Quit: 3\n", 8);
}

void	check_signals()
{
	signal(SIGINT, function_sigint);
	signal(SIGQUIT, function_sigwuit);
}

int main(int ac, char **av, char **env)
{
	(void)av;
	char		*temp;
	t_heredoc	*here;
	t_cmd		*lst;
	t_list		*list;
	char		*str;
	char		**res;

	g_signal_status = 0;
	here = malloc(sizeof(t_heredoc));
	lst = malloc(sizeof(t_cmd));
	list = malloc(sizeof(t_list));
	list->envs = env_init(env);
	if (ac != 1 || !lst || !list)
		return (1);
	while (1)
	{
		rl_catch_signals = 0;
		check_signals();
		temp = readline("Mouad_shell-$ ");
		if (!temp)
		{
			printf("exit\n");
			break ;
		}
		add_history(temp);
		if (syn_error(temp))
			continue ;
		str = add_space(temp);
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
		expand(lst, list);
		remove_qoutes(&lst);
		g_signal_status = 1;
		if (is_heredoc(lst, here))
			if (heredoc(lst, here))
				continue ;
			printf("%d\n", lst->outfile);
		execution(lst, list);
		g_signal_status = 0;
	}
}
