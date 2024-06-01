/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:40:25 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/31 21:36:08 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_flag(char *argv)
{
	int	i;

	i = 0;
	if (!ft_strcmp(argv, "-n"))
		return (0);
	if (argv[0] != '-')
		return (1);
	i++;
	while (argv[i])
	{
		if (argv[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	print_arguments(char **argv, int start, int outfile)
{
	int	i;

	i = start;
	while (argv[i])
	{
		write(outfile, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(outfile, " ", 1);
		i++;
	}
}

int	echo(char **argv, int outfile)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (argv[i] == NULL)
	{
		write(outfile, "\n", 1);
		return (0);
	}
	if (ft_strcmp(argv[i], "-") == 0)
	{
		write(outfile, argv[i], ft_strlen(argv[i]));
		write(outfile, " ", 1);
		i++;
	}
	while (argv[i] && !check_if_flag(argv[i]))
	{
		flag = 1;
		i++;
	}
	print_arguments(argv, i, outfile);
	if (!flag)
		write(outfile, "\n", 1);
	return (0);
}
