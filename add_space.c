/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_space.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:53:40 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/02 23:45:50 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	count_num_of_special(char *line)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if ((line[i] == '<' && line[i + 1] == '<')
			|| (line[i] == '>' && line[i + 1] == '>'))
		{
			count++;
			i += 2;
		}
		else if (is_special_char(line[i]))
		{
			count++;
			i++;
		}
		else
			i++;
	}
	return (count);
}

void	second_case(char *str, char *line, int *i, int *j)
{
	str[(*j)++] = ' ';
	str[(*j)++] = line[(*i)++];
	str[(*j)++] = line[(*i)++];
	str[(*j)++] = ' ';
}

char	*add_space(char *line)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	str = malloc(ft_strlen(line) + count_num_of_special(line) * 2 + 1);
	if (!str)
		return (NULL);
	while (line[i])
	{
		if ((line[i] == '<' && line[i + 1] == '<')
			|| (line[i] == '>' && line[i + 1] == '>'))
			second_case(str, line, &i, &j);
		else if (is_special_char(line[i]))
		{
			str[j++] = ' ';
			str[j++] = line[i++];
			str[j++] = ' ';
		}
		else
			str[j++] = line[i++];
	}
	str[j] = '\0';
	return (str);
}
