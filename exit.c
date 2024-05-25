/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:07:53 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/23 23:03:53 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int exit_status(int status, int mode)
// {
// 	static char num = 0;
// 	if (mode == 1)
// 		num = status;
// 	return (num);
// }

int	is_all_num(char *s)
{
	if (*s == '-' || *s == '+')
		s++;
	if (*s == '\0')
		return (0);
	while (*s)
	{
		if (!is_number(*s))
			return (0);
		s++;
	}
	return (1);
}

int	exit_function(char **argv)
{
	printf("exit\n");
	if (argv[1] == NULL)
		exit(0);
	if (is_all_num(argv[1]))
	{
		if (argv[2] == NULL)
		{
			if (ft_atoi(argv[1]) < 0)
				exit(256 + ft_atoi(argv[1]));
			else if (ft_atoi(argv[1]) > 255)
				exit(ft_atoi(argv[1]) % 256);
			else
				exit(ft_atoi(argv[1]));
		}
		else
		{
			printf("too many arguments\n");
			return (1);
		}
	}
	else
	{
		printf("%s: %s: numeric argument required\n", argv[0], argv[1]);
		exit(255);
	}
	return (0);
}
