/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:42:27 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/30 13:48:19 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Create export array entry
char	*create_export_array_entry(t_env *current)
{
	char	*entry;
	int		len;

	len = ft_strlen(current->key);
	if (current->value)
		len += ft_strlen(current->value) + 4;
	entry = malloc(len + 1);
	if (!entry)
		return (NULL);
	if (current->value == NULL)
	{
		ft_strlcpy(entry, current->key, len + 1);
	}
	else
	{
		ft_strlcpy(entry, current->key, len + 1);
		ft_strlcat(entry, "=\"", len + 1);
		ft_strlcat(entry, current->value, len + 1);
		ft_strlcat(entry, "\"", len + 1);
	}
	return (entry);
}

int	process_export_env_entry(t_env *current, char **env_array, int index)
{
	if (should_skip_env_var(current))
		return (index);
	env_array[index] = create_export_array_entry(current);
	if (!env_array[index])
	{
		cleanup_env_array(env_array, index);
		return (-1);
	}
	return (index + 1);
}

// Convert environment linked list to array for export

char	**env_to_array_export(t_env *env)
{
	int		count;
	t_env	*current;
	char	**env_array;
	int		i;

	count = count_export_vars(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		i = process_export_env_entry(current, env_array, i);
		if (i == -1)
			return (NULL);
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
