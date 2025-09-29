/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:27:08 by rboland           #+#    #+#             */
/*   Updated: 2025/03/18 13:27:08 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token(char *value, int type, int quoted_state)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->quoted_state = quoted_state;
	token->next = NULL;
	return (token);
}

// Add token to the end of the list
void	add_token(t_token **tokens, t_token *new)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new;
}

// Handle quotes (both single and double)
char	*extract_quoted_str(char *input, int *i, char quote_char)
{
	int		start;
	int		end;
	char	*result;

	start = *i + 1;
	end = start;
	while (input[end] && input[end] != quote_char)
		end++;
	if (!input[end])
	{
		*i = end;
		return (ft_strdup(&input[start]));
	}
	*i = end + 1;
	result = malloc(end - start + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, &input[start], end - start + 1);
	return (result);
}
