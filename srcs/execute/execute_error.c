/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:35:12 by rboland           #+#    #+#             */
/*   Updated: 2025/04/28 17:35:12 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Handle fork error */
int	handle_fork_error(char *exec_path, int heredoc_fd)
{
	perror("fork");
	free(exec_path);
	if (heredoc_fd != STDIN_FILENO)
		close(heredoc_fd);
	return (1);
}

/* Close unneeded file descriptors */
void	close_unneeded_fds(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
}

/* Restore standard file descriptors */
void	restore_std_fds(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}
