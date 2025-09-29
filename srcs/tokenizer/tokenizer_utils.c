/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <romain.boland@hotmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:24:21 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 13:07:19 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Process pipe token
void	process_pipe_token(t_token **tokens, int *i)
{
	add_token(tokens, new_token("|", TOKEN_PIPE, 0));
	(*i)++;
}

// Process input redirection (<)
void	process_input_redir(t_token **tokens, char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		if (input[*i + 2] == '<')
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `<'",
				STDERR_FILENO);
			*i = -1;
			return ;
		}
		add_token(tokens, new_token("<<", TOKEN_HEREDOC, 0));
		*i += 2;
	}
	else
	{
		add_token(tokens, new_token("<", TOKEN_REDIR_IN, 0));
		(*i)++;
	}
}

// Process output redirection (>)
void	process_output_redir(t_token **tokens, char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		if (input[*i + 2] == '>')
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `>'",
				STDERR_FILENO);
			*i = -1;
			return ;
		}
		add_token(tokens, new_token(">>", TOKEN_APPEND, 0));
		*i += 2;
	}
	else
	{
		add_token(tokens, new_token(">", TOKEN_REDIR_OUT, 0));
		(*i)++;
	}
}

// Process quoted heredoc delimiter
void	process_quoted_heredoc(t_token **tokens, char *in, int *i, int q_type)
{
	int		start;
	char	*delim;
	char	quote_char;

	if (q_type == 1)
		quote_char = '\'';
	else
		quote_char = '"';
	(*i)++;
	start = *i;
	while (in[*i] && in[*i] != quote_char)
		(*i)++;
	delim = malloc(*i - start + 1);
	if (delim)
	{
		ft_strlcpy(delim, &in[start], *i - start + 1);
		add_token(tokens, new_token(delim, TOKEN_WORD, q_type));
		free(delim);
	}
	if (in[*i] == quote_char)
		(*i)++;
}

// Process unquoted heredoc delimiter
void	process_unquoted_heredoc(t_token **tokens, char *input, int *i)
{
	char	*delim;

	delim = extract_word(input, i);
	if (delim)
	{
		add_token(tokens, new_token(delim, TOKEN_WORD, 0));
		free(delim);
	}
}
