/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:36:15 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 17:43:13 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Create a new environment variable node
t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

// Split envp entry into key-value pair
void	split_env_entry(char *entry, char **key, char **value)
{
	char	*equals_sign;

	equals_sign = ft_strchr(entry, '=');
	if (equals_sign)
	{
		*equals_sign = '\0';
		*key = entry;
		*value = equals_sign + 1;
	}
	else
	{
		*key = entry;
		*value = NULL;
	}
}

// Add node to env list
void	add_env_node(t_env **env_list, t_env **current, t_env *new_node)
{
	if (!*env_list)
		*env_list = new_node;
	else
		(*current)->next = new_node;
	*current = new_node;
}

// Process single env variable
void	process_env_entry(char *entry, t_env **env_list, t_env **current)
{
	char	*key;
	char	*value;
	t_env	*new_node;

	split_env_entry(entry, &key, &value);
	new_node = create_env_node(key, value);
	if (!new_node)
		return ;
	add_env_node(env_list, current, new_node);
}

// Create single env array entry
char	*create_env_array_entry(char *key, char *value)
{
	char	*entry;
	size_t	key_len;
	size_t	val_len;

	key_len = ft_strlen(key);
	val_len = 0;
	if (value)
		val_len = ft_strlen(value);
	entry = malloc(key_len + val_len + 2);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, key, key_len + 1);
	ft_strlcat(entry, "=", key_len + 2);
	if (value)
		ft_strlcat(entry, value, key_len + val_len + 2);
	return (entry);
}
