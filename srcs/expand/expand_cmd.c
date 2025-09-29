/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:25:31 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 19:25:31 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Expand variables in a command's arguments */
void	expand_command_args(t_command *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		if (cmd->arg_quoted[i] == 1)
		{
			i++;
			continue ;
		}
		expanded = expand_variables(cmd->args[i], shell);
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
	expand_redirections(cmd, shell);
	expand_heredoc_delims(cmd, shell);
}

/* Expand redirections */
void	expand_redirections(t_command *cmd, t_shell *shell)
{
	t_redirection	*redir;
	char			*expanded;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type != TOKEN_HEREDOC)
		{
			expanded = expand_variables(redir->file, shell);
			if (expanded)
			{
				free(redir->file);
				redir->file = expanded;
			}
		}
		redir = redir->next;
	}
}

/* Expand heredoc delimiters */
void	expand_heredoc_delims(t_command *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (i < cmd->heredoc_count)
	{
		if (cmd->heredoc_delims[i])
		{
			expanded = expand_variables(cmd->heredoc_delims[i], shell);
			if (expanded)
			{
				free(cmd->heredoc_delims[i]);
				cmd->heredoc_delims[i] = expanded;
			}
		}
		i++;
	}
}

/* Expand variables in a pipeline of commands */
void	expand_pipeline(t_pipeline *pipeline, t_shell *shell)
{
	int	i;

	if (!pipeline)
		return ;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		expand_command_args(pipeline->commands[i], shell);
		i++;
	}
}

/* Process a character inside double quotes */
char	*process_dquote_char(char *result, char c, int *i)
{
	char	str[2];
	char	*temp;

	str[0] = c;
	str[1] = '\0';
	temp = result;
	result = ft_strjoin(result, str);
	free(temp);
	(*i)++;
	return (result);
}
