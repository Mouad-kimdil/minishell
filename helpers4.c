/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:24:10 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/17 04:48:01 by mkimdil          ###   ########.fr       */
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

void	handling_my_argv(t_cmd *node)
{
	int		i;
	int		j;
	char	**new_argv;
	int		count;

	count = count_argv(node);
	i = 0;
	j = 0;
	new_argv = malloc((count + 1) * sizeof(char *));
	if (!new_argv)
		exit(EXIT_FAILURE);
	i = 0;
	while (node->argv[i] != NULL)
	{
		if (strlen(node->argv[i]) > 0)
		{
			new_argv[j] = node->argv[i];
			j++;
		}
		i++;
	}
	new_argv[j] = NULL;
	node->argv = new_argv;
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
