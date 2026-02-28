#include "minishell.h"

void	waits(t_execute *exec)
{
	int		status;
	int		last_status;
	pid_t	reaped;

	last_status = 0;
	while (1)
	{
		reaped = waitpid(-1, &status, 0);
		if (reaped <= 0)
			break ;
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = WTERMSIG(status) + 128;
		if (reaped == exec->id)
			last_status = status;
	}
	exit_status(last_status, 1);
}

void	my_execve(t_cmd *node, char **envr)
{
	if (execve(node->cmd, node->argv, envr) == -1)
	{
		if (!ft_strncmp(node->cmd, ".", 1))
		{
			write(2, "minishell: .: filename argument required\n", 41);
			exit_status(2, 1);
			exit(2);
		}
		else if (!ft_strcmp(node->cmd, "/"))
		{
			write(2, "minishell: /: is a directory\n", 29);
			exit_status(126, 1);
			exit(126);
		}
		else
		{
			msg_error("minishell");
			exit_status(126, 1);
			exit(126);
		}
	}
}

void	handle_last_command(t_cmd *node, t_list *list, t_execute *exec, char **envr)
{
	int	st;

	if (!check_for_redirection(node))
	{
		exec->id = safe_fork();
		if (exec->id == 0)
		{
			reset_signals_child();
			apply_redirs(node);
			if (!node->argv[0])
			{
				close_all(node, exec);
				exit(0);
			}
			st = is_builtin(node, list);
			if (st >= 0)
			{
				close_all(node, exec);
				exit(st);
			}
			free(node->cmd);
			node->cmd = command(node->argv[0], envr);
			if (!node->cmd || !ft_strcmp(node->cmd, ".."))
			{
				write(2, "minishell: command not found\n", 29);
				exit_status(127, 1);
				exit(127);
			}
			my_execve(node, envr);
		}
	}
}

void	handle_commands(t_cmd *node, t_list *list, t_execute *exec, char **envr)
{
	int	st;

	if (pipe(exec->fd) == -1)
		msg_error("pipe");
	if (!check_for_redirection(node))
	{
		exec->id = safe_fork();
		if (!exec->id)
		{
			reset_signals_child();
			apply_redirs_and_pipe_out(node, exec);
			if (!node->argv[0])
				exit(0);
			if (checkbuiltin(node))
			{
				st = is_builtin(node, list);
				if (st >= 0)
					exit(st);
			}
			free(node->cmd);
			node->cmd = command(node->argv[0], envr);
			if (!node->cmd)
			{
				write(2, "minishell: command not found\n", 29);
				exit_status(127, 1);
				exit(127);
			}
			my_execve(node, envr);
		}
		else
			close(node->inf);
	}
}

void	execute_pipeline(t_cmd *node, t_list *list)
{
	char		**envr;
	t_execute	exec;

	exec.saved_stdin = dup(0);
	exec.saved_stdout = dup(1);
	envr = env_to_char_array(list->envs);
	if (check_if_built(node, list, &exec))
	{
		free_str_array(envr);
		return ;
	}
	while (node->next)
	{
		handle_commands(node, list, &exec, envr);
		if (node->inf != 0)
			close(node->inf);
		if (node->outfile != 1)
			close(node->outfile);
		close((&exec)->fd[1]);
		dup2((&exec)->fd[0], 0);
		close((&exec)->fd[0]);
		node = node->next;
	}
	handle_last_command(node, list, &exec, envr);
	close_all(node, &exec);
	free_str_array(envr);
	waits(&exec);
}
