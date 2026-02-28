#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	if (g_signal_status == 0)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		exit_status(130, 1);
	}
	else
	{
		write(1, "\n", 1);
		exit_status(130, 1);
	}
}

void	handle_sigquit(int sig)
{
	(void)sig;
	if (g_signal_status == 0)
		return ;
	write(1, "Quit: 3\n", 8);
	exit_status(131, 1);
}

void	check_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	reset_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
