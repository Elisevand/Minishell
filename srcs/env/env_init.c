/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:36:47 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 17:40:36 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_shlvl_from_envp(char **envp)
{
	int	i;
	int	shlvl;

	i = 0;
	shlvl = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			shlvl = ft_atoi(envp[i] + 6);
		i++;
	}
	return (shlvl);
}

// Process SHLVL
t_env	*process_shlvl(t_env **env_list, t_env **current,
	int shlvl, int is_in_minishell)
{
	t_env	*new_node;
	char	shlvl_str[12];

	if (is_in_minishell)
		shlvl++;
	if (shlvl < 0)
		shlvl = 0;
	sprintf(shlvl_str, "%d", shlvl);
	new_node = create_env_node("SHLVL", shlvl_str);
	if (!new_node)
		return (*env_list);
	add_env_node(env_list, current, new_node);
	return (new_node);
}

// Add minishell flag
t_env	*add_minishell_flag(t_env **env_list, t_env **current)
{
	t_env	*new_node;

	new_node = create_env_node("is_in_minishell", "true");
	if (!new_node)
		return (*env_list);
	if (*current)
		(*current)->next = new_node;
	else if (!*env_list)
		*env_list = new_node;
	return (new_node);
}

// Initialize environment variables from envp
t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*current;
	int		i;
	int		is_in_minishell;
	int		shlvl;

	env_list = NULL;
	current = NULL;
	i = 0;
	is_in_minishell = 0;
	if (!envp)
		return (NULL);
	shlvl = get_shlvl_from_envp(envp);
	while (envp[i])
	{
		if (check_special_env_var(envp[i], "is_in_minishell=true", 21))
			is_in_minishell = 1;
		else if (!check_special_env_var(envp[i], "SHLVL=", 6))
			process_env_entry(envp[i], &env_list, &current);
		i++;
	}
	current = process_shlvl(&env_list, &current, shlvl, is_in_minishell);
	current = add_minishell_flag(&env_list, &current);
	return (env_list);
}
