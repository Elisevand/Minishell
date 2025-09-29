/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:22:09 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/25 15:39:17 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Count number of commands in the pipeline
int	count_commands(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 1;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

// Create a new command structure
t_command	*create_command(t_token *tokens)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->arg_quoted = NULL;
	cmd->redirections = NULL;
	cmd->has_heredoc = 0;
	cmd->heredoc_count = 0;
	cmd->heredoc_delims = NULL;
	cmd->heredoc_quoted = NULL;
	cmd->token = tokens;
	return (cmd);
}

static void	heredoc_redirection(t_command *cmd, char *file, int quoted_state)
{
	char	**new_delim;
	int		*new_quoted;
	int		i;

	new_delim = malloc(sizeof(char *) * (cmd->heredoc_count + 1));
	if (!new_delim)
		return ;
	i = -1;
	while (++i < cmd->heredoc_count)
		new_delim[i] = cmd->heredoc_delims[i];
	new_delim[cmd->heredoc_count] = ft_strdup(file);
	new_quoted = malloc(sizeof(int) * (cmd->heredoc_count + 1));
	if (!new_quoted)
		return (free(new_delim));
	i = -1;
	while (++i < cmd->heredoc_count)
		new_quoted[i] = cmd->heredoc_quoted[i];
	new_quoted[cmd->heredoc_count] = quoted_state;
	if (cmd->heredoc_delims)
		free(cmd->heredoc_delims);
	if (cmd->heredoc_quoted)
		free(cmd->heredoc_quoted);
	cmd->heredoc_delims = new_delim;
	cmd->heredoc_quoted = new_quoted;
	cmd->heredoc_count++;
}

void	add_redirection(t_command *cmd, char *file, int type, int quoted_state)
{
	t_redirection	*new_redir;
	t_redirection	*current;

	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
		return ;
	new_redir->file = ft_strdup(file);
	new_redir->type = type;
	new_redir->quoted = quoted_state;
	new_redir->next = NULL;
	if (!cmd->redirections)
		cmd->redirections = new_redir;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	if (type == TOKEN_HEREDOC)
	{
		cmd->has_heredoc = 1;
		heredoc_redirection(cmd, file, quoted_state);
	}
}
