/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:37:05 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 17:44:15 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Check if specific env var exists
int	check_special_env_var(char *entry, char *var_name, int name_len)
{
	if (ft_strncmp(entry, var_name, name_len) == 0)
		return (1);
	return (0);
}

// Count environment variables
int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

// Skip underscore var
int	should_skip_env_var(t_env *current)
{
	if (ft_strcmp(current->key, "_") == 0 && current->value != NULL)
		return (1);
	return (0);
}

// Count exportable env vars
int	count_export_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (!should_skip_env_var(env))
			count++;
		env = env->next;
	}
	return (count);
}
