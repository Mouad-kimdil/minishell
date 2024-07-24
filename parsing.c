/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:58:20 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/22 04:43:45 by mkimdil          ###   ########.fr       */
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
	if (c == '>' || c == '<' || c == '|')
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
	if (c == '<' || c == '>' || c == '|' || c == '`'
		|| c == '(' || c == ')' || c == '#' || c == ';')
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

int	syn_error(char *line)
{
	char	**res;
	int		i;

	res = ft_split(line, ' ');
	if (!res || !*res)
		return (1);
	i = 0;
	while (res[i])
	{
		if ((res[i][0] == '>' || res[i][0] == '<'
			|| double_red(res[i]) || res[i][0] == '|') 
			&& (!res[i + 1] || syn_err_chars(res[i + 1][0])))
			return (ft_putstr_fd("bash: syntax error\n", 2) , 1);
		if (res[0][0] == '|')
			return (ft_putstr_fd("bash: syntax error\n", 2) , 1);
		if (is_red(res[i][0]) && syn_err_chars(res[i + 1][0]))
			return (ft_putstr_fd("bash: syntax error\n", 2) , 1);
		if (ft_strchr(res[i], '`') || ft_strchr(res[i], ')')
			|| ft_strchr(res[i], '('))
			return (ft_putstr_fd("bash: syntax error\n", 2) , 1);
		i++;
	}
	return (0);
}
