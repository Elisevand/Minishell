/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:49:25 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:49:25 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Create process for a command in pipeline */
pid_t	create_command_process(t_pipeline *pipeline, t_shell *shell,
		int i, t_pipeline_iter *iter)
{
	pid_t		pid;
	t_child_ctx	child_ctx;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		child_ctx = init_child_ctx(pipeline, shell, i, iter);
		execute_pipeline_child(pipeline, shell, i, &child_ctx);
	}
	return (pid);
}

/* Close other heredocs in child process */
void	close_other_heredocs(t_pipeline *pipeline, int i, int *heredoc_fds)
{
	int	j;

	j = 0;
	while (j < pipeline->cmd_count)
	{
		if (j != i && heredoc_fds[j] != STDIN_FILENO)
			close(heredoc_fds[j]);
		j++;
	}
}

/* Execute command in child process */
void	execute_pipeline_child(t_pipeline *pipeline, t_shell *shell,
		int i, t_child_ctx *ctx)
{
	setup_child_process_signals(pipeline->commands[i]);
	close_other_heredocs(pipeline, i, ctx->heredoc_fds);
	free(ctx->heredoc_fds);
	handle_child_redirections(pipeline, i, ctx->in_fd, ctx->out_fd);
	execute_child_process(pipeline, shell, i);
}

/* Handle child process redirections */
void	handle_child_redirections(t_pipeline *pipeline, int i,
		int in_fd, int out_fd)
{
	int				input_redirected;

	input_redirected = handle_input_redirections(pipeline->commands[i]);
	if (!input_redirected)
		setup_command_input(i, in_fd, pipeline);
	handle_output_redirections(pipeline->commands[i], out_fd);
}

/* Setup command input */
void	setup_command_input(int i, int in_fd, t_pipeline *pipeline)
{
	if (i > 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	else if (pipeline->commands[i]->heredoc_count > 0
		&& pipeline->commands[i]->has_heredoc)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
}
