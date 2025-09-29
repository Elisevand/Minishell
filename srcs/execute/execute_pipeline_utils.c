/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:49:04 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:49:04 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Setup heredocs for all commands in pipeline */
int	setup_heredocs(t_pipeline *pipeline, t_shell *shell, int *heredoc_fds)
{
	int	i;

	i = 0;
	while (i < pipeline->cmd_count)
	{
		if (pipeline->commands[i]->has_heredoc)
		{
			heredoc_fds[i] = setup_heredoc(pipeline->commands[i], shell);
			if (heredoc_fds[i] < 0)
			{
				cleanup_heredocs(heredoc_fds, i);
				return (0);
			}
		}
		else
			heredoc_fds[i] = STDIN_FILENO;
		i++;
	}
	return (1);
}

/* Cleanup heredocs on error */
void	cleanup_heredocs(int *heredoc_fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (heredoc_fds[i] != STDIN_FILENO)
			close(heredoc_fds[i]);
		i++;
	}
}
