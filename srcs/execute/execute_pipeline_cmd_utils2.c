/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_cmd_utils2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <romain.boland@hotmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:38:41 by rboland           #+#    #+#             */
/*   Updated: 2025/05/01 15:39:15 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Initialize pipe for pipeline execution */
int	init_pipeline_pipe(t_pipeline *pipeline, int pipefds[2][2])
{
	if (pipeline->cmd_count > 1)
	{
		if (pipe(pipefds[0]) < 0)
			return (0);
	}
	return (1);
}

// Garde le nom de la commande dans le cas d'un chemin absolu
char	*get_basename(char *path)
{
	char	*last_slash;

	if (!path)
		return (NULL);
	last_slash = ft_strrchr(path, '/');
	if (last_slash)
		return (last_slash + 1);
	return (path);
}

// Verifie que c'est une blocking command comme cat et grep
int	is_blocking_command(char *cmd)
{
	char	*base;

	if (!cmd)
		return (0);
	base = get_basename(cmd);
	return (
		ft_strcmp(base, "cat") == 0
		|| ft_strcmp(base, "grep") == 0
		|| ft_strcmp(base, "more") == 0
		|| ft_strcmp(base, "less") == 0
		|| ft_strcmp(base, "tail") == 0
		|| ft_strcmp(base, "head") == 0
		|| ft_strcmp(base, "wc") == 0
		|| ft_strcmp(base, "awk") == 0
		|| ft_strcmp(base, "sed") == 0
		|| ft_strcmp(base, "sort") == 0
		|| ft_strcmp(base, "cut") == 0
		|| ft_strcmp(base, "tr") == 0
	);
}

/* Execute a single command in the pipeline */
int	execute_pipeline_cmd(t_pipeline *pipeline, t_shell *shell,
	int i, t_pipeline_iter *iter)
{
	t_child_ctx	child_ctx;

	iter->pids[i] = fork();
	if (iter->pids[i] < 0)
	{
		perror("fork");
		return (0);
	}
	if (iter->pids[i] == 0)
	{
		if (is_blocking_command(pipeline->commands[i]->args[0]))
			setup_child_process_signals(pipeline->commands[i]);
		close_unused_pipes(pipeline, i, iter->active_pipe, iter->pipefds);
		setup_pipe_redirects(iter->in_fd, iter->out_fd);
		child_ctx = init_child_ctx(pipeline, shell, i, iter);
		execute_pipeline_command_child(pipeline, shell, i, &child_ctx);
	}
	handle_parent_pipes(i, iter);
	return (1);
}
