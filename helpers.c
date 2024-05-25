/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:53:55 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/23 19:02:15 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_files(t_cmd *node)
{
	if (node->infile != 0)
		close(node->infile);
	if (node->outfile != 1)
		close(node->outfile);
}

// void	handle_builtin_command(t_cmd *node, int fd_int, int fd_out)
// {
// 	if (node->infile != 0)
// 		close(node->infile);
// 	if (node->outfile != 1)
// 		close(node->outfile);
// 	dup2(fd_int, 0);
// 	close(fd_int);
// 	dup2(fd_out, 0);
// 	close(fd_out);
// 	return ;
// }

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
