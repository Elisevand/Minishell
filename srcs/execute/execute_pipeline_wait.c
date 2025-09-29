/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_wait.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:50:00 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:50:00 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Wait for all commands in pipeline to complete */
int	wait_for_pipeline(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	exit_status;

	i = cmd_count - 1;
	exit_status = 0;
	while (i >= 0)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = 128 + WTERMSIG(status);
			else
				exit_status = 1;
		}
		i--;
	}
	return (exit_status);
}
