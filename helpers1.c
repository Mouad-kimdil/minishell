/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:55:17 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/24 17:33:01 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_duplications(t_cmd *node, int *fd)
// {
// 	if (node->infile != 0)
// 	{
// 		if (dup2(node->infile, 0) == -1)
// 			msg_error("dup2 infile");
// 		close(node->infile);
// 	}
// 	if (node->outfile != 1)
// 	{
// 		if (dup2(node->outfile, 1) == -1)
// 			msg_error("dup2 outfile");
// 		close(node->outfile);
// 	}
// 	else if (fd != NULL)
// 	{
// 		close(fd[0]);
// 		if (dup2(fd[1], 1) == -1)
// 			msg_error("dup2 fd[1]");
// 		close(fd[1]);
// 	}
// }

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
