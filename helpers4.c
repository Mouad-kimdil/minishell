/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:24:10 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/20 22:26:35 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_delim_size(t_cmd *lst)
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

int	count_argv(t_cmd *node)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (node->argv[i] != NULL)
	{
		if (ft_strlen(node->argv[i]) > 0)
			count++;
		i++;
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
