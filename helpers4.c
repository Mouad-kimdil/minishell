/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:24:10 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/01 04:00:43 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	her_sin(int sig)
{
	if (sig == SIGINT)
	{
		close(0);
		ex_st(1, 1);
	}
}

int	is_heredoc(t_cmd *lst)
{
	int	i;
	int	res;

	res = 0;
	while (lst)
	{
		i = 0;
		lst->is_heredoc = 0;
		while (lst->argv[i] && lst->argv[i + 1])
		{
			if (!ft_strcmp(lst->argv[i], "<<")
				&& ft_strcmp(lst->argv[i + 1], "<"))
			{
				lst->is_heredoc = 1;
				res = 1;
			}
			i++;
		}
		lst = lst->next;
	}
	return (res);
}

int	get_del_size(t_cmd *lst)
{
	int	count;
	int	i;

	count = 0;
	while (lst)
	{
		i = 0;
		while (lst->argv[i])
		{
			if (!ft_strcmp(lst->argv[i], "<<"))
				count++;
			i++;
		}
		lst = lst->next;
	}
	return (count);
}

int	ft_isspace(char str)
{
	while (str)
	{
		if (str == ' ' || str == '\t')
			return (1);
		str++;
	}
	return (0);
}

int	is_blank(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}
