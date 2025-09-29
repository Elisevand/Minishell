/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:07:24 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/25 15:08:22 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_cmdargs(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
}

static void	free_cmdheredocs(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->heredoc_count)
	{
		free(cmd->heredoc_delims[i]);
		i++;
	}
	free(cmd->heredoc_delims);
	free(cmd->heredoc_quoted);
}

// Free command structure
void	free_command(t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	*next_redir;

	if (!cmd)
		return ;
	if (cmd->args)
		free_cmdargs(cmd);
	if (cmd->arg_quoted)
		free(cmd->arg_quoted);
	redir = cmd->redirections;
	while (redir)
	{
		next_redir = redir->next;
		free(redir->file);
		free(redir);
		redir = next_redir;
	}
	if (cmd->heredoc_delims)
		free_cmdheredocs(cmd);
	free(cmd);
}

// Free pipeline structure
void	free_pipeline(t_pipeline *pipeline)
{
	int	i;

	if (!pipeline)
		return ;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		free_command(pipeline->commands[i]);
		i++;
	}
	free(pipeline->commands);
	free(pipeline);
}
