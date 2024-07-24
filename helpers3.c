/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:18:27 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/22 22:43:54 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_dup1(t_cmd *node, t_execute *exec)
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
	close(exec->fd[0]);
	if (dup2(exec->fd[1], 1) == -1)
		msg_error("dup2 in fd[1]");
	close(exec->fd[1]);
}

void	my_dup2(t_cmd *node)
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
}
