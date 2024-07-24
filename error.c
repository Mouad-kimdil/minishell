/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:50:50 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/23 01:56:15 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strsearch(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (i < ft_strlen(s) + 1)
	{
		if (s[i] == (char)c)
			return (1);
		i++;
	}
	return (0);
}
