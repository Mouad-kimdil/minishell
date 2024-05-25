/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:39:12 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/23 23:07:25 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(char **args, t_list *list, int outfile)
{
	char	*pwdir;
	char	*tmp;

	(void)args;
	pwdir = my_getenv("PWD", list);
	if (!pwdir)
	{
		pwdir = getcwd(NULL, 0);
		tmp = my_getenv("OLDPWD", list);
		if (!pwdir)
		{
			printf("%s\n", tmp);
			return ;
		}
	}
	write(outfile, pwdir, ft_strlen(pwdir));
	write(outfile, "\n", 1);
}

void	error_open(char *str)
{
	perror(str);
	return ;
}