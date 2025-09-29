/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_redirect.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:32:06 by rboland           #+#    #+#             */
/*   Updated: 2025/04/28 17:32:06 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Setup input/output redirections for pipeline command */
void	setup_pipe_redirects(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

/* Handle input redirections */
int	handle_input_redirections(t_command *cmd)
{
	t_redirection	*redir;
	int				fd;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd < 0)
			{
				perror(redir->file);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			return (1);
		}
		redir = redir->next;
	}
	return (0);
}

/* Process a redirection of type REDIR_OUT */
int	process_out_redir(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

/* Process a redirection of type APPEND */
int	process_append_redir(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}
