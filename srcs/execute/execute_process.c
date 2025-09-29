/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:47:44 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:47:44 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Process redirections for command */
int	setup_cmd_redirections(t_command *cmd)
{
	t_redirection	*redir;
	int				fd;

	redir = cmd->redirections;
	while (redir)
	{
		fd = 0;
		if (redir->type == TOKEN_REDIR_IN)
			fd = setup_input_redir(redir);
		else if (redir->type == TOKEN_REDIR_OUT)
			fd = setup_output_redir(redir, O_TRUNC);
		else if (redir->type == TOKEN_APPEND)
			fd = setup_output_redir(redir, O_APPEND);
		if (redir->type == TOKEN_REDIR_IN
			|| redir->type == TOKEN_REDIR_OUT
			|| redir->type == TOKEN_APPEND)
		{
			if (fd < 0)
				return (0);
		}
		redir = redir->next;
	}
	return (1);
}

/* Setup input redirection */
int	setup_input_redir(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->file);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (fd);
}

/* Setup output redirection */
int	setup_output_redir(t_redirection *redir, int mode)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | mode, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}

/* Initialize child process context */
t_child_process_ctx	init_child_process_ctx(t_command *cmd,
							int fds[2], char *exec_path, t_shell *shell)
{
	t_child_process_ctx	ctx;

	ctx.cmd = cmd;
	ctx.in_fd = fds[0];
	ctx.out_fd = fds[1];
	ctx.exec_path = exec_path;
	ctx.shell = shell;
	return (ctx);
}

/* Child process execution */
void	child_process(t_command *cmd, t_child_process_ctx *ctx)
{
	setup_child_process_signals(cmd);
	if (!setup_redirections(cmd, ctx->in_fd, ctx->out_fd))
	{
		free(ctx->exec_path);
		exit(1);
	}
	close_unneeded_fds(ctx->in_fd, ctx->out_fd);
	execute_child_command(cmd, ctx->exec_path, ctx->shell);
}
