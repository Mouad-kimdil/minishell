/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:55:09 by mkimdil           #+#    #+#             */
/*   Updated: 2024/05/30 15:10:18 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_args(t_cmd *lst)
{
	int	i;

	while (lst)
	{
		i = 0;
		while (lst->argv[i])
		{
			printf("lst->argv[%d] : %s\n", i, lst->argv[i]);
			printf("lst->cmd[%d] : %s\n", i, lst->cmd);
			i++;
		}
		lst = lst->next;
	}
}

int	parse_heredoc(t_cmd *lst)
{
	while (lst)
	{
		if (ft_strnstr(lst->cmd, "<<  <") || ft_strnstr(lst->cmd, "<<  >"))
			return (1);
		lst = lst->next;
	}
	return (0);
}

void	execute_here_doc(t_cmd *lst, t_list *list)
{
	
}

void	here_doc(t_cmd *lst, t_list *list)
{
	int	pid;

	(void) list;
	if (parse_heredoc(lst))
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
		execute_here_doc(lst, list);
}

int	is_heredoc(t_cmd *lst, t_list *list)
{
	print_args(lst);
	(void) list;
	while (lst)
	{
		if (ft_strnstr(lst->cmd, "<<") == 1)
			return (1);
		lst = lst->next;
	}
	return (0);
}
