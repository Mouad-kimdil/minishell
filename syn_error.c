/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syn_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 02:44:18 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/14 04:18:29 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	double_red(char *s)
{
	if (!ft_strcmp(s, "<<") || !ft_strcmp(s, ">>"))
		return (1);
	return (0);
}

int	is_red(int c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	last_check(int c)
{
	if (c == '`' || c == '(' || c == ')')
		return (1);
	return (0);
}

int	syn_err_chars(int c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

int	check_line(char **res)
{
	int	i;
	int	j;
	int	tr;

	i = 0;
	tr = 0;
	while (res[i])
	{
		j = 0;
		tr = 0;
		while (res[i][j])
		{
			if (res[i][j] == '<')
				tr++;
			j++;
		}
		if (tr > 2)
			return (1);
		i++;
	}
	return (0);
}
