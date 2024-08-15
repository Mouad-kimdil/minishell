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
