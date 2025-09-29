/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:59:31 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 13:59:31 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Setup pipe for next command */
int	setup_next_pipe(t_pipeline *pipeline, int i, int pipefds[2][2],
		int active_pipe)
{
	if (i < pipeline->cmd_count - 2)
	{
		if (pipe(pipefds[1 - active_pipe]) < 0)
			return (0);
	}
	return (1);
}

/* Set output file descriptor for pipeline command */
static int	set_command_output(t_pipeline *pipeline, int i, int active_pipe,
				int pipefds[2][2])
{
	if (i == pipeline->cmd_count - 1)
		return (STDOUT_FILENO);
	else
		return (pipefds[active_pipe][1]);
}

/* Create error context */
static t_pipe_error	create_error_context(t_pipeline *pipeline, int i,
					t_pipeline_iter *iter)
{
	t_pipe_error	ctx;

	ctx.pipeline = pipeline;
	ctx.i = i;
	ctx.in_fd = iter->in_fd;
	ctx.heredoc_fds = iter->heredoc_fds;
	ctx.pids = iter->pids;
	ctx.active_pipe = iter->active_pipe;
	ft_memcpy(ctx.pipefds, iter->pipefds, sizeof(iter->pipefds));
	return (ctx);
}

/* Handle error in pipe iteration */
int	handle_pipe_iter_error(t_pipeline *pipeline, int i,
		t_pipeline_iter *iter)
{
	t_pipe_error	ctx;

	ctx = create_error_context(pipeline, i, iter);
	handle_pipe_error(ctx.heredoc_fds, ctx.pipeline->cmd_count);
	if (ctx.i > 0)
		close(ctx.in_fd);
	close(ctx.pipefds[ctx.active_pipe][0]);
	close(ctx.pipefds[ctx.active_pipe][1]);
	if (ctx.i < ctx.pipeline->cmd_count - 2)
	{
		close(ctx.pipefds[1 - ctx.active_pipe][0]);
		close(ctx.pipefds[1 - ctx.active_pipe][1]);
	}
	wait_for_processes(ctx.pids, ctx.i);
	return (0);
}

/* Single iteration of pipeline execution */
int	execute_pipeline_iter(t_pipeline *pipeline, t_shell *shell,
	int i, t_pipeline_iter *iter)
{
	int	result;

	iter->out_fd = set_command_output(pipeline, i, iter->active_pipe,
			iter->pipefds);
	if (i < pipeline->cmd_count - 1 && !setup_next_pipe(pipeline, i,
			iter->pipefds, iter->active_pipe))
		return (handle_pipe_iter_error(pipeline, i, iter));
	result = execute_pipeline_cmd(pipeline, shell, i, iter);
	if (!result)
		return (0);
	iter->active_pipe = 1 - iter->active_pipe;
	return (1);
}
