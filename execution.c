/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:59:50 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/24 17:39:15 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

/*
	if command not foud the exit status is 127
	if command is found but not executable the exit status is 126 
			for scriptig and permission denied
	if command is successful the exit status is 0
	if command was error the exit status is 1

*/
int	is_builtin(t_cmd *cmd, t_list *list)
{
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (echo(cmd->argv, cmd->outfile), 1);
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (cd(cmd->argv, list), 1);
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (pwd(cmd->argv, list, cmd->outfile), 1);
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (export(cmd->argv, list, cmd->outfile), 1);
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (unset(cmd->argv, &list->envs), 1);
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (env(cmd->argv, list, cmd->outfile), 1);
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

void	execution(t_cmd *node, t_list *list)
{
	int		fd[2];
	int		id;
	int		fd_int;
	int		fd_out;
	char	**envr;

	fd_int = dup(0);
	fd_out = dup(1);
	envr = env_to_char_array(list->envs);
	if (!node->next)
	{
		if (checkbuiltin(node))
		{
			check_for_redirection(node);
			if (is_builtin(node, list))
			{
				if (node->infile != 0)
					close(node->infile);
				if (node->outfile != 1)
					close(node->outfile);
				dup2(fd_int, 0);
				close(fd_int);
				dup2(fd_out, 1);
				close(fd_out);
				return ;
			}
		}
	}
	while (node->next)
	{
		if (check_for_redirection(node) == 1)
		{
			if (node->infile != 0)
				close(node->infile);
			if (node->outfile != 1)
				close(node->outfile);
			dup2(fd_int, 0);
			close(fd_int);
			dup2(fd_out, 1);
			close(fd_out);
			return ;
		}
		if (pipe(fd) == -1)
			msg_error("pipe");
		id = safe_fork();
		if (id == 0)
		{
			if (node->infile != 0)
			{
				if (dup2(node->infile, 0) == -1)
					msg_error("dup2 in infile");
			}
			if (node->outfile != 1)
			{
				if (dup2(node->outfile, 1) == -1)
					msg_error("dup2 in outfile");
			}
			else
			{
				close(fd[0]);
				if (dup2(fd[1], 1) == -1)
					msg_error("dup2 in fd[1]");
				close(fd[1]);
			}
			if (checkbuiltin(node))
			{
				is_builtin(node, list);
				exit(0);
			}
			node->cmd = command(node->argv[0], envr);
			if (!node->cmd)
			{
				printf("minishell: %s: command not found\n", node->argv[0]);
				exit(127);
			}
			if (execve(node->cmd, node->argv, envr) == -1)
			{
				msg_error("execve");
				exit(126);
			}
		}
		if (node->infile != 0)
			close(node->infile);
		if (node->outfile != 1)
			close(node->outfile);
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		node = node->next;
	}
	if (check_for_redirection(node) == 1)
	{
		if (node->infile != 0)
			close(node->infile);
		if (node->outfile != 1)
			close(node->outfile);
		dup2(fd_int, 0);
		close(fd_int);
		dup2(fd_out, 1);
		close(fd_out);
		return ;
	}
	id = safe_fork();
	if (id == 0)
	{
		if (node->infile != 0)
		{
			if (dup2(node->infile, 0) == -1)
				msg_error("dup2 in infile2");
			close(node->infile);
		}
		if (node->outfile != 1)
		{
			if (dup2(node->outfile, 1) == -1)
				msg_error("dup2 in outfile2");
			close(node->outfile);
		}
		if (is_builtin(node, list))
		{
			if (node->infile != 0)
				close(node->infile);
			if (node->outfile != 1)
				close(node->outfile);
			dup2(fd_int, 0);
			close(fd_int);
			dup2(fd_out, 1);
			close(fd_out);
			exit(0);
		}
		node->cmd = command(node->argv[0], envr);
		if (!node->cmd)
		{
			printf("minishell: %s: command not found\n", node->argv[0]);
			exit(127);
		}
		if (execve(node->cmd, node->argv, envr) == -1)
		{
			perror("minishell");
			exit(126);
		}
	}
	if (node->infile != 0)
		close(node->infile);
	if (node->outfile != 1)
		close(node->outfile);
	dup2(fd_int, 0);
	close(fd_int);
	dup2(fd_out, 1);
	close(fd_out);
	while (wait(NULL) != -1)
		;
}
