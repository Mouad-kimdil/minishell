/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:58:20 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/01 04:54:21 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syn_error(char *line)
{
	char	**res;
	int		i;

	res = ft_split_2(line);
	if (!res || !*res)
		return (1);
	i = 0;
	while (res[i])
	{
		if ((res[i][0] == '>' || res[i][0] == '<'
			|| double_red(res[i]) || res[i][0] == '|')
			&& (!res[i + 1] || syn_err_chars(res[i + 1][0])))
			return (free_all(res), put_fd("bash: syntax error\n", 2), 1);
		if (res[0][0] == '|')
			return (free_all(res), put_fd("bash: syntax error\n", 2), 1);
		if (is_red(res[i][0]) && syn_err_chars(res[i + 1][0]))
			return (free_all(res), put_fd("bash: syntax error\n", 2), 1);
		if (ft_strchr(res[i], '`') || ft_strchr(res[i], ')')
			|| ft_strchr(res[i], '('))
			return (free_all(res), put_fd("bash: syntax error\n", 2), 1);
		i++;
	}
	return (free_all(res), 0);
}
