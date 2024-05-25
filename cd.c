/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:54:47 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/22 14:03:02 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	home_function(char *home, t_list *list)
{
	home = my_getenv("HOME", list);
	if (!home)
	{
		printf("cd: HOME not set\n");
		return ;
	}
	if (chdir(home) == -1)
		printf("cd: %s: No such file or directory\n", home);
	update_pwd(my_getenv("HOME", list), list);
}

void	home_function_telda(char *home, t_list *list)
{
	home = my_getenv("HOME", list);
	if (!home)
		home = "/Users/aboukdid";
	if (chdir(home) == -1)
		printf("cd: %s: No such file or directory\n", home);
	update_pwd(my_getenv("HOME", list), list);
}

void	old_pwd_function(char *home, t_list *list)
{
	(void)home;
	if (chdir(my_getenv("OLDPWD", list)) == -1)
		printf("cd: %s: No such file or directory\n",
			my_getenv("OLDPWD", list));
	update_pwd(my_getenv("OLDPWD", list), list);
	printf("%s\n", my_getenv("PWD", list));
}

void	error_function(char *home, t_list *list)
{
	home = my_getenv("HOME", list);
	if (!home)
		printf("cd: HOME not set\n");
	if (chdir(home) == -1)
		printf("cd: %s: No such file or directory\n", home);
	printf("cd: error retrieving current directory: ");
	printf("getcwd: cannot access parent directories: ");
	printf ("No such file or directory\n");
	update_pwd(my_getenv("HOME", list), list);
}

int	cd(char **argv, t_list *list)
{
	int		i;
	char	*home;

	i = 1;
	home = NULL;
	while (argv[i])
		i++;
	if (i == 1)
		return (home_function(home, list), 0);
	else
	{
		if (!ft_strcmp(argv[1], "~"))
			return (home_function_telda(home, list), 0);
		else if (!ft_strcmp(argv[1], "-"))
			return (old_pwd_function(home, list), 0);
		else if (chdir(argv[1]) == -1)
		{
			if (!ft_strcmp(argv[1], ".."))
				return (error_function(home, list), 0);
			printf("cd: %s: No such file or directory\n", argv[1]);
			return (1);
		}
	}
	update_pwd(argv[1], list);
	return (0);
}
