#include "minishell.h"

void	close_files(t_cmd *node)
{
	if (node->inf != 0)
		close(node->inf);
	if (node->outfile != 1)
		close(node->outfile);
}

void	safe_pipe(int fd[2])
{
	if (pipe(fd) == -1)
		msg_error("pipe");
}

void	msg_error_fork(void)
{
	static int	error_printed;

	if (!error_printed)
	{
		perror("fork");
		error_printed = 1;
	}
	else
		return ;
}

int	safe_fork(void)
{
	int	id;

	id = fork();
	if (id == -1)
		msg_error_fork();
	return (id);
}

void	msg_error(char *str)
{
	perror(str);
	return ;
}

char	*ft_strjoin_with_sep(char *s1, char *s2, char sep)
{
	int		i;
	int		j;
	char	*result;

	i = -1;
	if (!s1 || !s2)
		return (NULL);
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!result)
		return (NULL);
	while (s1[++i])
		result[i] = s1[i];
	result[i] = sep;
	j = i + 1;
	i = -1;
	while (s2[++i])
		result[j + i] = s2[i];
	result[j + i] = '\0';
	return (result);
}

int	check(char *my_argv)
{
	if (!my_argv)
		return (1);
	if (my_argv[0] == '/' || my_argv[0] == '.')
	{
		if (access(my_argv, F_OK | X_OK) == 0)
			return (1);
		else
		{
			msg_error("minishell");
			exit_status(126, 1);
			exit(126);
		}
	}
	return (0);
}

char	*command(char *my_argv, char **envr)
{
	char	**path;
	char	*joiner;
	char	*command_path;
	int		i;

	if (check(my_argv))
		return (my_argv);
	path = get_path(envr);
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		if (access(my_argv, F_OK | X_OK) == 0)
			return (my_argv);
		joiner = ft_strjoin(path[i], "/");
		command_path = ft_strjoin(joiner, my_argv);
		free(joiner);
		if (access(command_path, F_OK | X_OK) == 0)
			return (free_str_array(path), command_path);
		free(command_path);
		i++;
	}
	return (free_str_array(path), NULL);
}

void	free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			free(arr[i]);
			i++;
		}
		free(arr);
	}
}

char	**get_path(char **envr)
{
	int		i;
	char	**s;

	i = 0;
	while (envr[i])
	{
		if (!ft_strncmp("PATH=", envr[i], 5))
			break ;
		i++;
	}
	if (!envr[i])
		return (NULL);
	s = ft_split(envr[i] + 5, ':');
	return (s);
}

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
