#include "minishell.h"

void	my_dup1(t_cmd *node, t_execute *exec)
{
	if (node->inf != 0)
	{
		if (dup2(node->inf, 0) == -1)
			msg_error("dup2 in inf");
		close(node->inf);
	}
	if (node->outfile != 1)
	{
		if (dup2(node->outfile, 1) == -1)
			msg_error("dup2 in outfile");
		close(node->outfile);
	}
	else
	{
		close(exec->fd[0]);
		if (dup2(exec->fd[1], 1) == -1)
			msg_error("dup2 in fd[1]");
		close(exec->fd[1]);
	}
}

void	my_dup2(t_cmd *node)
{
	if (node->inf != 0)
	{
		if (dup2(node->inf, 0) == -1)
			msg_error("dup2 in inf");
		close(node->inf);
	}
	if (node->outfile != 1)
	{
		if (dup2(node->outfile, 1) == -1)
			msg_error("dup2 in outfile");
		close(node->outfile);
	}
}
