/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:15:15 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/13 00:36:54 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status(int status, int mode)
{
	static int	num;

	if (mode == 1)
		num = status;
	return (num);
}

int	is_builtin(t_cmd *cmd, t_list *list)
{
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (echo(cmd->argv), 1);
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (cd(cmd->argv, list), 1);
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (pwd(cmd->argv, list), 1);
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (export(cmd->argv, list), 1);
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (unset(cmd->argv, &list->envs), 1);
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (env(cmd->argv, list), 1);
	if (!ft_strcmp(cmd->argv[0], "exit"))
		return (exit_function(cmd->argv), 1);
	return (0);
}

int	checkbuiltin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "exit"))
		return (1);
	return (0);
}

int	check_if_built(t_cmd *node, t_list *list, t_execute *exec)
{
	if (!node->next)
	{
		if (checkbuiltin(node))
		{
			if (check_for_redirection(node))
			{
				close_all(node, exec);
				return (1);
			}
			my_dup2(node);
			if (is_builtin(node, list))
			{
				if (node->inf != 0)
					close(node->inf);
				if (node->outfile != 1)
					close(node->outfile);
				dup2(exec->fd_int, 0);
				close(exec->fd_int);
				dup2(exec->fd_out, 1);
				close(exec->fd_out);
				return (1);
			}
		}
	}
	return (0);
}

void	close_all(t_cmd *node, t_execute *exec)
{
	if (node->inf != 0)
		close(node->inf);
	if (node->outfile != 1)
		close(node->outfile);
	dup2(exec->fd_int, 0);
	close(exec->fd_int);
	dup2(exec->fd_out, 1);
	close(exec->fd_out);
}
