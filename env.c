/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:03:27 by aboukdid          #+#    #+#             */
/*   Updated: 2024/06/03 16:53:37 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_split_helper(char *s, char c, char **result, int *i)
{
	while (s[*i] && s[*i] != c)
	{
		result[0][*i] = s[*i];
		(*i)++;
	}
	result[0][*i] = '\0';
	if (s[*i])
	{
		result[1] = ft_strdup(s + *i + 1);
		if (!result[1])
			return ;
	}
	else
		result[1] = NULL;
	result[2] = NULL;
}

char	**env_split(char *s, char c)
{
	char	**result;
	int		i;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char *) * 3);
	if (!result)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
		i++;
	result[0] = malloc(sizeof(char) * (i + 1));
	if (!result[0])
		return (free(result), NULL);
	i = 0;
	env_split_helper(s, c, result, &i);
	return (result);
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	int		i;
	char	**sp;
	t_env	*new;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		sp = env_split(envp[i], '=');
		if (!sp)
			break ;
		if (sp[1] != NULL)
		{
			new = ft_lstnew(sp[0], sp[1]);
			if (new == NULL)
				break ;
			ft_lstadd_back(&head, new);
		}
		free_all(sp);
		i++;
	}
	return (head);
}

void	env(char **argv, t_list *list, int outfile)
{
	t_env	*env;

	(void)argv;
	env = list->envs;
	while (env)
	{
		if (env->value)
		{
			write(outfile, env->name, ft_strlen(env->name));
			write(outfile, "=", 1);
			write(outfile, env->value, ft_strlen(env->value));
			write(outfile, "\n", 1);
		}
		env = env->next;
	}
	ex_st(0, 1);
}

void	env_1(char **argv, t_env *list, int outfile)
{
	(void) *argv;
	while (list)
	{
		if (list->value)
		{
			write(outfile, list->name, ft_strlen(list->name));
			write(outfile, "=", 1);
			write(outfile, list->value, ft_strlen(list->value));
			write(outfile, "\n", 1);
		}
		list = list->next;
	}
}
