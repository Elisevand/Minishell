/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:28:31 by rboland           #+#    #+#             */
/*   Updated: 2025/03/18 13:28:31 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Handle syntax validation */
static int	validate_input_syntax(t_token *tokens, t_shell *shell)
{
	if (!validate_syntax(tokens) || !check_consecutive_redirections(tokens))
	{
		shell->last_exit_status = 2;
		free_tokens(tokens);
		return (0);
	}
	return (1);
}

/* Parse tokens and handle errors */
static t_pipeline	*parse_input_tokens(t_token *tokens)
{
	t_pipeline	*pipeline;

	pipeline = parse_tokens(tokens);
	if (!pipeline)
	{
		printf("Error: Failed to parse tokens\n");
		free_tokens(tokens);
		return (NULL);
	}
	return (pipeline);
}

/* Execute pipeline and update status */
static void	execute_input_pipeline(t_pipeline *pipeline, t_shell *shell)
{
	int	status;

	status = execute_pipeline(pipeline, shell);
	shell->last_exit_status = status;
	setup_signals();
}

// Process the input line
void	process_input(char *input, t_shell *shell)
{
	t_token		*tokens;
	t_pipeline	*pipeline;

	tokens = tokenize_input(input);
	if (!tokens)
		return ;
	if (!validate_input_syntax(tokens, shell))
		return ;
	pipeline = parse_input_tokens(tokens);
	if (!pipeline)
		return ;
	expand_pipeline(pipeline, shell);
	execute_input_pipeline(pipeline, shell);
	free_tokens(tokens);
	free_pipeline(pipeline);
}
