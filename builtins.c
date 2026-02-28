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
		return (echo(cmd->argv));
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (cd(cmd->argv, list));
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (pwd(cmd->argv, list), 0);
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (export(cmd->argv, list), 0);
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (unset(cmd->argv, &list->envs));
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (env(cmd->argv, list), 0);
	if (!ft_strcmp(cmd->argv[0], "exit"))
		return (exit_function(cmd->argv));
	return (-1);
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
	int	status;

	if (!node->next && node->argv[0])
	{
		if (checkbuiltin(node))
		{
			if (check_for_redirection(node))
			{
				close_all(node, exec);
				return (1);
			}
			apply_redirs(node);
			status = is_builtin(node, list);
			if (status >= 0)
			{
				if (node->inf != 0)
					close(node->inf);
				if (node->outfile != 1)
					close(node->outfile);
				dup2(exec->saved_stdin, 0);
				close(exec->saved_stdin);
				dup2(exec->saved_stdout, 1);
				close(exec->saved_stdout);
				exit_status(status, 1);
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
	dup2(exec->saved_stdin, 0);
	close(exec->saved_stdin);
	dup2(exec->saved_stdout, 1);
	close(exec->saved_stdout);
}
