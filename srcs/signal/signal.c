/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <romain.boland@hotmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:20:20 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 13:32:12 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;

// Signal handler for interactive mode
void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		if (rl_readline_state & RL_STATE_READCMD)
		{
			if (tcgetpgrp(STDIN_FILENO) == getpgrp())
			{
				write(STDOUT_FILENO, "\n", 1);
				rl_on_new_line();
				rl_replace_line("", 0);
				rl_redisplay();
			}
		}
	}
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = signal_handler;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
