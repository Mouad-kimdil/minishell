/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:12:28 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/02 18:31:04 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*nops_strdup(char *str)
{
	int		i;
	int		j;
	char	*ptr;

	i = 0;
	while (str[i] == ' ')
		i++;
	ptr = (char *)malloc(ft_strlen(str) + 2 - i);
	if (!ptr)
		return (NULL);
	j = 0;
	while (str[i])
	{
		ptr[j] = str[i];
		i++;
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}

int	ft_strcpy(char *dest, char *src)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen((char *)src);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (len);
}

int	ft_strnstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i + j] && (str[i + j] == to_find[j]))
			j++;
		if (to_find[j] == '\0')
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi(char *str)
{
	size_t	r;
	int		i;
	int		s;

	r = 0;
	i = 0;
	s = 1;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - '0');
		if (r > 9223372036854775807ULL && s > 0)
			return (-1);
		else if (r > 922337203685477580ULL + 1 && s < 0)
			return (0);
		i++;
	}
	return (r * s);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = (char *)malloc(ft_strlen(str) + 1);
	if (!ptr)
		return (NULL);
	while (str[i])
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
