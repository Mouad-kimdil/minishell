/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:25:50 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/13 00:36:54 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	function_sigint(int sig)
{
	if (sig == SIGINT && g_signal_status == 0)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		exit_status(1, 1);
	}
}

void	function_sigwuit(int sig)
{
	if (sig == SIGQUIT && g_signal_status == 1)
		write(1, "Quit: 3\n", 8);
}

void	check_signals(void)
{
	signal(SIGINT, function_sigint);
	signal(SIGQUIT, function_sigwuit);
}
