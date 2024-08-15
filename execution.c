#include "minishell.h"

void	waits(t_execute *exec)
{
	int	last_status;
	int	status;

	if (waitpid(exec->id, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = WTERMSIG(status) + 128;
		while (wait(&status) != -1)
		{
			if (WIFEXITED(status))
				exit_status(WEXITSTATUS(status), 1);
			if (WIFSIGNALED(status))
				exit_status(WTERMSIG(status) + 128, 1);
		}
		exit_status(last_status, 1);
	}
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

void	hand_l_command(t_cmd *node, t_list *list, t_execute *exec, char **envr)
{
	if (!check_for_redirection(node))
	{
		exec->id = safe_fork();
		if (exec->id == 0)
		{
			my_dup2(node);
			if (is_builtin(node, list))
			{
				close_all(node, exec);
				exit(0);
			}
			if (!node->argv[0])
				exit(0);
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
	if (pipe(exec->fd) == -1)
		msg_error("pipe");
	if (!check_for_redirection(node))
	{
		exec->id = safe_fork();
		if (!exec->id)
		{
			my_dup1(node, exec);
			if (checkbuiltin(node))
			{
				is_builtin(node, list);
				exit(0);
			}
			if (!node->argv[0])
				exit(0);
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

void	ex(t_cmd *node, t_list *list)
{
	char		**envr;
	t_execute	exec;

	exec.fd_int = dup(0);
	exec.fd_out = dup(1);
	envr = env_to_char_array(list->envs);
	if (check_if_built(node, list, &exec))
	{
		fr(envr);
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
	hand_l_command(node, list, &exec, envr);
	close_all(node, &exec);
	fr(envr);
	waits(&exec);
}
