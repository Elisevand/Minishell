/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:25:23 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 19:25:23 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Check if a character is valid in a variable name */
int	is_valid_var_char(char c, int first_char)
{
	if (first_char)
		return (ft_isalpha(c) || c == '_');
	else
		return (ft_isalnum(c) || c == '_');
}

/* Extract a variable name from a string starting at index i */
char	*extract_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*var_name;

	start = *i;
	len = 0;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && is_valid_var_char(str[*i], len == 0))
	{
		(*i)++;
		len++;
	}
	if (len == 0)
		return (NULL);
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, (str + start + 1), (len + 1));
	return (var_name);
}

/* Expand a single variable and return its value */
char	*expand_single_var(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name(str, i);
	if (!var_name)
		return (ft_strdup(""));
	if (!ft_strncmp(var_name, "?", 2))
	{
		free(var_name);
		return (ft_itoa(shell->last_exit_status));
	}
	var_value = get_env_value(shell->env, var_name);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

/* Check if a position in a string is inside double quotes */
int	is_in_dquotes(char *str, int pos)
{
	int	i;
	int	in_dquotes;

	i = 0;
	in_dquotes = 0;
	while (i < pos)
	{
		if (str[i] == '"' && (i == 0 || str[i - 1] != '\\'))
			in_dquotes = !in_dquotes;
		i++;
	}
	return (in_dquotes);
}

/* Helper to handle variable check before processing */
int	should_expand_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1]
		&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
			|| str[i + 1] == '?'));
}
