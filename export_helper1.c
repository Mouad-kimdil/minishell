/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:39:00 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/16 13:13:28 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checking(t_env *tmp, char *name, char *value)
{
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
		{
			if (value != NULL)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
			}
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	add_env(t_env **env, char *name, char *value)
{
	t_env	*new_node;
	t_env	*tmp;

	tmp = *env;
	if (checking(tmp, name, value))
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->name = ft_strdup(name);
	if (!value)
		new_node->value = NULL;
	else
		new_node->value = ft_strdup(value);
	new_node->index = 0;
	new_node->next = NULL;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

int	add_the_value(char *name, char *value, t_list *list)
{
	t_env	*env;

	env = list->envs;
	if (!value)
		return (1);
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

int	update_the_value(char *name, char *value, t_list *list)
{
	t_env	*env;
	char	*temp;

	env = list->envs;
	if (!value)
		return (0);
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			temp = env->value;
			env->value = ft_strjoin(temp, value);
			return (0);
		}
		env = env->next;
	}
	return (1);
}
