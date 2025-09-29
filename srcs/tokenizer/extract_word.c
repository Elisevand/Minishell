/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <romain.boland@hotmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:57:11 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 13:13:37 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_quotes(char *input, int *end, int *in_single_quotes,
	int *in_double_quotes)
{
	if (input[*end] == '\'' && !(*in_double_quotes))
	{
		*in_single_quotes = !(*in_single_quotes);
		(*end)++;
	}
	else if (input[*end] == '\"' && !(*in_single_quotes))
	{
		*in_double_quotes = !(*in_double_quotes);
		(*end)++;
	}
	else
		(*end)++;
}

/* Check if we should stop processing the word */
static int	should_stop_word(char *input, int end, int in_single_quotes,
	int in_double_quotes)
{
	if (!in_single_quotes && !in_double_quotes
		&& ft_strchr(" \t\n|<>", input[end]))
		return (1);
	return (0);
}

/* Extract a word token with proper quote handling */
char	*extract_word(char *input, int *i)
{
	int		start;
	int		end;
	int		in_single_q;
	int		in_double_q;
	char	*result;

	start = *i;
	end = start;
	in_single_q = 0;
	in_double_q = 0;
	while (input[end])
	{
		if (input[end] == '\'' || input[end] == '\"')
			handle_quotes(input, &end, &in_single_q, &in_double_q);
		else if (should_stop_word(input, end, in_single_q, in_double_q))
			break ;
		else
			end++;
	}
	*i = end;
	result = malloc(end - start + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, &input[start], end - start + 1);
	return (result);
}
