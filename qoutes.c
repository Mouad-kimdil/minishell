/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qoutes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:44:40 by mkimdil           #+#    #+#             */
/*   Updated: 2024/05/19 16:53:52 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_double(char *line)
{
	int	i;
	int	tr;

	i = 0;
	tr = 0;
	while (line[i])
	{
		if (line[i] == '\'' && tr == 0)
			tr = 2;
		else if (line[i] == '\'' && tr == 2)
			tr = 0;
		else if (line[i] == '"' && tr == 0)
			tr = 1;
		else if (line[i] == '"' && tr == 1)
			tr = 0;
		i++;
	}
	if (tr == 1 || tr == 2)
		return (printf("syntax error\n"), 1);
	return (0);
}

void	change_to_garb(char *line)
{
	int		i;
	int		tr;
	char	curr_quote;

	i = 0;
	tr = 0;
	curr_quote = '\0';
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"') && tr == 0)
		{
			tr = 1;
			curr_quote = line[i];
		}
		else if (line[i] == curr_quote && tr == 1)
		{
			tr = 0;
			curr_quote = '\0';
		}
		else if (tr == 1)
			line[i] = line[i] * -1;
		i++;
	}
}

void	back_to_ascii(t_cmd *lst)
{
	int	i;

	while (lst)
	{
		change_to_garb(lst->cmd);
		i = 0;
		while (lst->argv[i])
		{
			change_to_garb(lst->argv[i]);
			i++;
		}
		lst = lst->next;
	}
}
