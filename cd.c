/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:54:47 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/13 00:36:54 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	home_function(char *home, t_list *list)
{
	home = my_getenv("HOME", list);
	if (!home)
	{
		printf("cd: HOME not set\n");
		exit_status(1, 1);
		return ;
	}
	if (chdir(home) == -1)
	{
		printf("cd: %s: No such file or directory\n", home);
		exit_status(1, 1);
	}
	update_pwd(list);
	exit_status(0, 1);
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
	exit_status(1, 1);
	update_pwd(list);
}

t_env	*find_env(t_env *envs, char *key)
{
	while (envs)
	{
		if (ft_strcmp(envs->name, key) == 0)
			return (envs);
		envs = envs->next;
	}
	return (NULL);
}

void	update_oldpwd(t_list *list, char *old_pwd)
{
	t_env	*oldpwd_env;
	char	*new_value;

	oldpwd_env = find_env(list->envs, "OLDPWD");
	if (oldpwd_env)
	{
		free(oldpwd_env->value);
		if (!old_pwd)
			new_value = getcwd(NULL, 0);
		else
			new_value = ft_strdup(old_pwd);
		oldpwd_env->value = new_value;
	}
	else
	{
		if (!old_pwd)
			old_pwd = getcwd(NULL, 0);
		oldpwd_env = ft_lstnew("OLDPWD", old_pwd);
		ft_lstadd_back(&list->envs, oldpwd_env);
	}
}

int	cd(char **argv, t_list *list)
{
	int		i;
	char	*home;
	char	*old_pwd;

	i = 1;
	old_pwd = getcwd(NULL, 0);
	home = NULL;
	while (argv[i])
		i++;
	if (i == 1)
		return (home_function(home, list), free(old_pwd), 0);
	if (chdir(argv[1]) == -1)
	{
		if (!ft_strcmp(argv[1], ".."))
			return (error_function(home, list), free(old_pwd), 0);
		else if (!ft_strcmp(argv[1], "\0"))
			return (exit_status(0, 1), free(old_pwd), 0);
		printf("cd: %s: No such file or directory\n", argv[1]);
		free(old_pwd);
		return (exit_status(1, 1), 1);
	}
	update_pwd(list);
	update_oldpwd(list, old_pwd);
	return (free(old_pwd), exit_status(0, 1), 0);
}
