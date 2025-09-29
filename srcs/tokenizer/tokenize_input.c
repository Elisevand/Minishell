/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <romain.boland@hotmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:55:20 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 13:08:55 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_special_chars(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '|')
	{
		process_pipe_token(tokens, i);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
			process_heredoc(tokens, input, i);
		else
			process_input_redir(tokens, input, i);
		if (*i == -1)
			return (0);
	}
	else if (input[*i] == '>')
	{
		process_output_redir(tokens, input, i);
		if (*i == -1)
			return (0);
	}
	return (1);
}

/* Cleanup and return NULL on error */
static t_token	*cleanup_on_error(t_token *tokens)
{
	free_tokens(tokens);
	return (NULL);
}

/* Main tokenization function - part 1 */
static int	init_tokenization(char *input, t_token **tokens, int *i)
{
	*tokens = NULL;
	*i = 0;
	if (has_unclosed_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes",
			STDERR_FILENO);
		return (0);
	}
	return (1);
}

/* Process a single token from input */
static int	process_token(t_token **tokens, char *input, int *i)
{
	if (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	else if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		if (!handle_special_chars(tokens, input, i))
			return (0);
	}
	else
		process_word_token(tokens, input, i);
	return (1);
}

/* Main tokenization function */
t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	int		i;

	if (!init_tokenization(input, &tokens, &i))
		return (NULL);
	while (input[i])
	{
		if (!process_token(&tokens, input, &i))
			return (cleanup_on_error(tokens));
	}
	return (tokens);
}
