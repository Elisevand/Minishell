/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:36:56 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 17:45:20 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Get the value of an environment variable 
char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

// Update existing env variable
void	update_env_var(t_env *env, char *value)
{
	free(env->value);
	if (value)
		env->value = ft_strdup(value);
	else
		env->value = NULL;
}

// Handle case where env_head is first node to be replaced
void	handle_first_node_replacement(t_env *env_head, t_env *new_node)
{
	char	*temp_key;
	char	*temp_value;
	t_env	*temp_next;

	temp_key = env_head->key;
	temp_value = env_head->value;
	temp_next = env_head->next;
	env_head->key = new_node->key;
	env_head->value = new_node->value;
	env_head->next = new_node->next;
	new_node->key = temp_key;
	new_node->value = temp_value;
	new_node->next = temp_next;
	env_head->next = new_node;
}

// Set an environment variable's value (or create if it doesn't exist)
void	set_env_value(t_env *env_head, char *key, char *value)
{
	t_env	*env;
	t_env	*prev;
	t_env	*new_node;

	env = env_head;
	prev = NULL;
	if (!env_head)
		return ;
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
		{
			update_env_var(env, value);
			return ;
		}
		prev = env;
		env = env->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return ;
	if (prev)
		prev->next = new_node;
	else
		handle_first_node_replacement(env_head, new_node);
}

// Check if an environment variable exists
int	has_env_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}
