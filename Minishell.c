/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 04:01:04 by mkimdil           #+#    #+#             */
/*   Updated: 2024/05/25 20:28:18 by mkimdil          ###   ########.fr       */
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

int main(int ac, char **av, char **env)
{
	(void)av;
	char *temp;
	t_cmd	*lst;
	t_list	*list;
	char *str;
	char **res;

	lst = malloc(sizeof(t_cmd));
	list = malloc(sizeof(t_list));
	list->envs = env_init(env);
	if (ac != 1 || !lst || !list)
		return (1);
	while (1)
	{
		temp = readline("Minishell-$ ");
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
		remove_qoutes(&lst);
		expand(lst, list);
		execution(lst, list);
	}
}
