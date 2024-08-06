/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:39:12 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/04 18:00:11 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(char **args, t_list *list)
{
	char	*pwdir;
	char	*tmp;
	int		allocated;

	(void)args;
	allocated = 0;
	pwdir = my_getenv("PWD", list);
	if (!pwdir)
	{
		pwdir = getcwd(NULL, 0);
		allocated = 1;
		tmp = my_getenv("OLDPWD", list);
		if (!pwdir)
		{
			if (tmp)
				printf("%s\n", tmp);
			ex_st(1, 1);
			return ;
		}
	}
	write(1, pwdir, ft_strlen(pwdir));
	write(1, "\n", 1);
	if (allocated)
		free(pwdir);
	ex_st(0, 1);
}

void	error_open(char *str)
{
	perror(str);
	return ;
}
