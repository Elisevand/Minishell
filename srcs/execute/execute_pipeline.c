/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:48:26 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:48:26 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Prepare pipeline resources */
int	prepare_pipeline_resources(t_pipeline *pipeline, pid_t **pids,
		int **heredoc_fds)
{
	*pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	*heredoc_fds = malloc(sizeof(int) * pipeline->cmd_count);
	if (!*pids || !*heredoc_fds)
	{
		free(*pids);
		free(*heredoc_fds);
		return (0);
	}
	return (1);
}

/* Check if pipeline is a single builtin command */
int	is_single_builtin(t_pipeline *pipeline)
{
	return (pipeline->cmd_count == 1 && pipeline->commands[0]->args
		&& is_builtin(pipeline->commands[0]->args[0]));
}

/* Execute a pipeline of commands */
int	execute_pipeline(t_pipeline *pipeline, t_shell *shell)
{
	int		exit_status;
	pid_t	*pids;
	int		*heredoc_fds;

	if (!pipeline || pipeline->cmd_count == 0)
		return (1);
	if (is_single_builtin(pipeline))
		return (execute_command(pipeline->commands[0],
				STDIN_FILENO, STDOUT_FILENO, shell));
	if (!prepare_pipeline_resources(pipeline, &pids, &heredoc_fds))
		return (1);
	if (!setup_heredocs(pipeline, shell, heredoc_fds))
	{
		free(pids);
		free(heredoc_fds);
		return (1);
	}
	exit_status = execute_pipeline_commands(pipeline, shell, pids, heredoc_fds);
	free(pids);
	free(heredoc_fds);
	return (exit_status);
}

/* Initialize pipeline iteration */
static t_pipeline_iter	init_pipeline_iteration(t_init_pipeline *init_ctx)
{
	t_pipeline_iter	iter;

	iter.in_fd = init_ctx->in_fd;
	iter.active_pipe = init_ctx->active_pipe;
	iter.pids = init_ctx->pids;
	iter.heredoc_fds = init_ctx->heredoc_fds;
	iter.pipeline = init_ctx->pipeline;
	return (iter);
}

/* Execute pipeline commands loop */
int	execute_pipeline_commands(t_pipeline *pipeline, t_shell *shell,
		pid_t *pids, int *heredoc_fds)
{
	int				i;
	t_pipeline_iter	iter;
	t_init_pipeline	init_ctx;

	i = 0;
	init_ctx.pipeline = pipeline;
	init_ctx.in_fd = STDIN_FILENO;
	init_ctx.active_pipe = 0;
	init_ctx.pids = pids;
	init_ctx.heredoc_fds = heredoc_fds;
	iter = init_pipeline_iteration(&init_ctx);
	if (!init_pipeline_pipe(pipeline, iter.pipefds))
		return (handle_pipe_error(heredoc_fds, pipeline->cmd_count));
	while (i < pipeline->cmd_count)
	{
		if (i == 0)
			iter.in_fd = STDIN_FILENO;
		else
			iter.in_fd = iter.pipefds[1 - iter.active_pipe][0];
		if (!execute_pipeline_iter(pipeline, shell, i, &iter))
			return (1);
		i++;
	}
	return (wait_for_pipeline(pids, pipeline->cmd_count));
}
