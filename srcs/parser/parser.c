/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:27:36 by rboland           #+#    #+#             */
/*   Updated: 2025/03/18 13:27:36 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_pipeline	*init_pipeline(t_token *tokens)
{
	t_pipeline	*pipeline;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	pipeline->cmd_count = count_commands(tokens);
	pipeline->commands = malloc(sizeof(t_command *) * pipeline->cmd_count);
	if (!pipeline->commands)
	{
		free(pipeline);
		return (NULL);
	}
	return (pipeline);
}

static int	handle_redirection(t_token *token, t_command *cmd)
{
	if (!token || !token->next || token->next->type != TOKEN_WORD)
		return (0);
	if (token->type == TOKEN_REDIR_IN)
		add_redirection(cmd, token->next->value, TOKEN_REDIR_IN,
			token->next->quoted_state);
	else if (token->type == TOKEN_REDIR_OUT)
		add_redirection(cmd, token->next->value, TOKEN_REDIR_OUT,
			token->next->quoted_state);
	else if (token->type == TOKEN_APPEND)
		add_redirection(cmd, token->next->value, TOKEN_APPEND,
			token->next->quoted_state);
	else if (token->type == TOKEN_HEREDOC)
		add_redirection(cmd, token->next->value, TOKEN_HEREDOC,
			token->next->quoted_state);
	else
		return (0);
	return (1);
}

static void	handle_token(t_token *token, t_pipeline *pipeline, int cmd_idx)
{
	if (token->type == TOKEN_WORD)
	{
		add_arg(pipeline->commands[cmd_idx], token->value,
			token->quoted_state);
	}
}

static t_token	*process_current_token(t_token *current,
		t_pipeline *pipeline, int *i)
{
	if (current->type == TOKEN_PIPE)
	{
		(*i)++;
		pipeline->commands[*i] = create_command(current);
		return (current->next);
	}
	else if (handle_redirection(current, pipeline->commands[*i]))
		return (current->next->next);
	else
	{
		handle_token(current, pipeline, *i);
		return (current->next);
	}
}

// Parse tokens into a pipeline of commands
t_pipeline	*parse_tokens(t_token *tokens)
{
	t_pipeline	*pipeline;
	t_token		*current;
	int			i;

	pipeline = init_pipeline(tokens);
	if (!pipeline)
		return (NULL);
	current = tokens;
	i = 0;
	pipeline->commands[i] = create_command(tokens);
	while (current)
		current = process_current_token(current, pipeline, &i);
	return (pipeline);
}
