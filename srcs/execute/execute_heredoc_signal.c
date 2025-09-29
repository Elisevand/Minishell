/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_signal.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:37:58 by rboland           #+#    #+#             */
/*   Updated: 2025/04/28 17:37:58 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Save and restore signal handlers for heredoc */
void	handle_heredoc_signals(struct sigaction *old_int,
							struct sigaction *old_quit)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
	ft_memset(&sa_int, 0, sizeof(sa_int));
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = SIG_IGN;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/* Setup child process signal handling for heredoc */
void	setup_child_signals(void)
{
	struct sigaction	sa_child_int;

	ft_memset(&sa_child_int, 0, sizeof(sa_child_int));
	sa_child_int.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_child_int, NULL);
}
