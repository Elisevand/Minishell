/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <romain.boland@hotmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:24:47 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 12:55:42 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Skip whitespace after a token
void	skip_whitespace(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

// Process heredoc token
void	process_heredoc(t_token **tokens, char *input, int *i)
{
	add_token(tokens, new_token("<<", TOKEN_HEREDOC, 0));
	*i += 2;
	skip_whitespace(input, i);
	if (input[*i] == '"')
		process_quoted_heredoc(tokens, input, i, 2);
	else if (input[*i] == '\'')
		process_quoted_heredoc(tokens, input, i, 1);
	else
		process_unquoted_heredoc(tokens, input, i);
}

// Process a normal word token
void	process_word_token(t_token **tokens, char *input, int *i)
{
	char	*token_value;

	token_value = extract_word(input, i);
	add_token(tokens, new_token(token_value, TOKEN_WORD, 0));
	free(token_value);
}

// Free token list
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
