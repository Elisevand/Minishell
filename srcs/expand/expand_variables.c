/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:35:07 by rboland           #+#    #+#             */
/*   Updated: 2025/04/24 19:35:07 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Process content inside single quotes */
char	*process_single_quotes(char *result, char *str, int *i)
{
	char	*temp;
	char	c[2];

	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		c[0] = str[*i];
		c[1] = '\0';
		temp = result;
		result = ft_strjoin(result, c);
		free(temp);
		(*i)++;
	}
	if (str[*i] == '\'')
		(*i)++;
	return (result);
}

/* Process a regular character */
char	*process_char(char *result, char c)
{
	char	*temp;
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	temp = result;
	result = ft_strjoin(result, str);
	free(temp);
	return (result);
}

/* Process content inside double quotes */
char	*process_double_quotes(char *result, char *str, int *i, t_shell *shell)
{
	char	*var_value;
	char	*temp;

	(*i)++;
	while (str[*i] && str[*i] != '\"')
	{
		if (should_expand_var(str, *i))
		{
			var_value = expand_single_var(str, i, shell);
			temp = result;
			result = ft_strjoin(result, var_value);
			free(temp);
			free(var_value);
		}
		else
			result = process_dquote_char(result, str[*i], i);
	}
	if (str[*i] == '\"')
		(*i)++;
	return (result);
}

/* Process variables outside of quotes */
char	*process_variable(char *result, char *str, int *i, t_shell *shell)
{
	char	*temp;
	char	*var_value;

	var_value = expand_single_var(str, i, shell);
	temp = result;
	result = ft_strjoin(result, var_value);
	free(temp);
	free(var_value);
	return (result);
}

/* Expand all environment variables in a string */
char	*expand_variables(char *str, t_shell *shell)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'')
			result = process_single_quotes(result, str, &i);
		else if (str[i] == '\"')
			result = process_double_quotes(result, str, &i, shell);
		else if (should_expand_var(str, i))
			result = process_variable(result, str, &i, shell);
		else
		{
			result = process_char(result, str[i]);
			i++;
		}
	}
	return (result);
}
