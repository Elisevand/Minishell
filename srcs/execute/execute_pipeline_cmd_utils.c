/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_cmd_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:25:06 by rboland           #+#    #+#             */
/*   Updated: 2025/04/28 17:25:06 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Initialize child context */
t_child_ctx	init_child_ctx(t_pipeline *pipeline, t_shell *shell,
	int i, t_pipeline_iter *iter)
{
	t_child_ctx	ctx;

	ctx.pipeline = pipeline;
	ctx.shell = shell;
	ctx.i = i;
	ctx.in_fd = iter->in_fd;
	ctx.out_fd = iter->out_fd;
	ctx.heredoc_fds = iter->heredoc_fds;
	return (ctx);
}

/* Handle parent process pipes */
void	handle_parent_pipes(int i, t_pipeline_iter *iter)
{
	if (i > 0)
		close(iter->in_fd);
	if (i < iter->pipeline->cmd_count - 1)
		close(iter->pipefds[iter->active_pipe][1]);
}

/* Close unused pipes in child process */
void	close_unused_pipes(t_pipeline *pipeline, int i, int active_pipe,
	int pipefds[2][2])
{
	if (i < pipeline->cmd_count - 1)
		close(pipefds[active_pipe][0]);
	if (i < pipeline->cmd_count - 2)
	{
		close(pipefds[1 - active_pipe][0]);
		close(pipefds[1 - active_pipe][1]);
	}
	if (i > 0)
		close(pipefds[1 - active_pipe][1]);
}
