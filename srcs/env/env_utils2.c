/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:37:16 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/30 13:48:12 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Remove an environment variable
void	remove_env_var(t_env *env, char *key)
{
	t_env	*prev;
	t_env	*current;

	prev = NULL;
	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	process_standard_env_entry(t_env *current, char **env_array, int index)
{
	if (current->value == NULL)
		return (index);
	env_array[index] = create_env_array_entry(current->key, current->value);
	if (!env_array[index])
	{
		cleanup_env_array(env_array, index);
		return (-1);
	}
	return (index + 1);
}

// Convert environment linked list to array for execve
// Convert environment linked list to array for execve
char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*current;
	char	**env_array;
	int		i;

	count = count_env_vars(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		i = process_standard_env_entry(current, env_array, i);
		if (i == -1)
			return (NULL);
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
