/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 04:17:36 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/29 15:01:36 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(int c)
{
	return (c == ' ' || c == '\t');
}

int	countword_2(char *s)
{
	int	count;

	count = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (!is_whitespace(*s))
		{
			count++;
			while (*s && !is_whitespace(*s))
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_help_2(char *s, int len, char **final)
{
	char	*start;
	int		i;

	len = countword_2(s);
	final = (char **)malloc((len + 1) * sizeof(char *));
	if (!final)
		return (NULL);
	i = 0;
	while (i < len)
	{
		while (is_whitespace(*s))
			s++;
		start = (char *)s;
		while (*s && !is_whitespace(*s))
			s++;
		final[i] = ft_strndup(start, s - start);
		if (!final[i])
		{
			ft_free(final, i);
			return (NULL);
		}
		i++;
	}
	final[i] = NULL;
	return (final);
}

char	**ft_split_2(char *s)
{
	int		len;
	char	**final;

	final = NULL;
	len = 0;
	return (ft_help_2(s, len, final));
}
