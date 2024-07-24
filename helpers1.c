/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:55:17 by aboukdid          #+#    #+#             */
/*   Updated: 2024/06/03 03:54:36 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_to_char_array_helper(t_env *current, char **envp)
{
	int	i;

	i = 0;
	while (current)
	{
		if (!current->value)
		{
			envp[i] = ft_strjoin_with_sep(current->name, "", '=');
			if (!envp[i])
				return ;
		}
		else
		{
			envp[i] = ft_strjoin_with_sep(current->name, current->value, '=');
			if (!envp[i])
				return ;
		}
		if (!envp[i])
		{
			free_all(envp);
			return ;
		}
		i++;
		current = current->next;
	}
}

char	**env_to_char_array(t_env *head)
{
	int		i;
	t_env	*current;
	char	**envp;

	i = 0;
	current = head;
	i = env_size(head);
	if (i == 0)
		return (NULL);
	envp = malloc((i + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	current = head;
	env_to_char_array_helper(current, envp);
	envp[i] = NULL;
	return (envp);
}
