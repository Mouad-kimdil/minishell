/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:20:55 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/23 22:40:33 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*my_getenv(char *name, t_list *list)
{
	t_env	*env;

	env = list->envs;
	while (env)
	{
		if (!ft_strcmp(env->name, name))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	update_env(char *name, char *value, t_list *list)
{
	t_env	*tmp;

	tmp = list->envs;
	if (!name || !value)
		return ;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, name, ft_strlen(name)))
		{
			tmp->value = ft_strdup(value);
			break ;
		}
		tmp = tmp->next;
	}
}

void	update_pwd(char *path, t_list *list)
{
	char	*home;

	path = NULL;
	update_env("OLDPWD", my_getenv("PWD", list), list);
	home = getcwd(NULL, 0);
	update_env("PWD", home, list);
}
