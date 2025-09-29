/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:40:29 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 19:00:21 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Check if a token is a redirection operator
int	is_redirection(int token_type)
{
	return (token_type == TOKEN_REDIR_IN
		|| token_type == TOKEN_REDIR_OUT
		|| token_type == TOKEN_APPEND
		|| token_type == TOKEN_HEREDOC);
}

/* Handle pipe syntax errors */
int	check_pipe_errors(t_token *current, t_token *prev)
{
	if (!prev || prev->type == TOKEN_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		return (0);
	}
	if (!current->next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		return (0);
	}
	return (1);
}

/* Print error for consecutive redirections */
void	print_redir_error(int token_type)
{
	const char	*token_str;

	token_str = NULL;
	if (token_type == TOKEN_REDIR_IN)
		token_str = "<";
	else if (token_type == TOKEN_REDIR_OUT)
		token_str = ">";
	else if (token_type == TOKEN_APPEND)
		token_str = ">>";
	else if (token_type == TOKEN_HEREDOC)
		token_str = "<<";
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)token_str, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

/* Handle errors for missing word after redirection */
int	handle_missing_word_error(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		return (0);
	}
	else if (current->next->type == TOKEN_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		return (0);
	}
	else if (is_redirection(current->next->type))
	{
		print_redir_error(current->next->type);
		return (0);
	}
	return (1);
}
