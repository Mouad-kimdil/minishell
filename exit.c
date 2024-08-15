/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:07:53 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/15 00:00:23 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	special_case(char c)
{
	return (is_ascii(c) || is_number(c) || c == '_');
}

int	args_len(char **arg)
{
	int	i;

	if (!arg || !*arg)
		return (0);
	i = 0;
	while (arg[i])
		i++;
	return (i);
}

int	is_all_num(char *s)
{
	while (*s == ' ')
		s++;
	if (*s == '-' || *s == '+')
		s++;
	if (*s == '\0')
		return (0);
	while (*s)
	{
		if (!is_number(*s) || *s == ' ')
			break ;
		s++;
	}
	while (*s && *s == ' ')
		s++;
	if (*s == '\0')
		return (1);
	return (0);
}

int	exit_functions_help(char *argv1, int state)
{
	if (state == -1)
	{
		exit_status(256 + ft_atoi(argv1), 1);
		exit(256 + ft_atoi(argv1));
	}
	else if (state == 1)
	{
		exit_status(ft_atoi(argv1) % 256, 1);
		exit(ft_atoi(argv1) % 256);
	}
	else
	{
		exit_status(ft_atoi(argv1), 1);
		exit(ft_atoi(argv1));
	}
}

int	exit_function(char **argv)
{
	printf("exit\n");
	if (argv[1] == NULL)
		exit(exit_status(0, 0));
	if (is_all_num(argv[1]))
	{
		if (argv[2] == NULL)
		{
			if (ft_atoi(argv[1]) == -1 || ft_atoi(argv[1]) == 0)
			{
				printf("%s: %s: numeric argument required\n", argv[0], argv[1]);
				exit_status(255, 1);
				exit(255);
			}
			if (ft_atoi(argv[1]) < 0)
				exit_functions_help(argv[1], -1);
			else if (ft_atoi(argv[1]) > 255)
				exit_functions_help(argv[1], 1);
			else
				exit_functions_help(argv[1], 0);
		}
		else
			return (printf("too many arguments\n"), 1);
	}
	printf("%s: %s: numeric argument required\n", argv[0], argv[1]);
	return (exit_status(255, 1), exit(255), 0);
}
