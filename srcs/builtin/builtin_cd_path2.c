/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_path2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:21:10 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/25 13:30:35 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Construit le chemin complet en joignant le chemin actuel et le nouveau
static char	*build_full_path(char *current_path, char *new_path)
{
	char	*temp_path;
	char	*temp;
	int		last_index;

	last_index = ft_strlen(current_path) - 1;
	if (current_path[last_index] == '/')
	{
		temp_path = ft_strjoin(current_path, new_path);
		return (temp_path);
	}
	temp = ft_strjoin(current_path, "/");
	if (!temp)
		return (NULL);
	temp_path = ft_strjoin(temp, new_path);
	free(temp);
	return (temp_path);
}

// Ajoute un composant au chemin avec la gestion spéciale pour la racine
static char	*append_path_component(char *path, char *component)
{
	char	*new_path;
	char	*temp;
	char	*component_with_slash;
	int		is_root;

	is_root = (path[0] == '/' && path[1] == '\0');
	temp = path;
	if (is_root)
	{
		component_with_slash = ft_strjoin(path, component);
	}
	else
	{
		component_with_slash = ft_strjoin("/", component);
		if (!component_with_slash)
			return (free(path), NULL);
		new_path = ft_strjoin(path, component_with_slash);
		free(component_with_slash);
		free(temp);
		return (new_path);
	}
	free(temp);
	return (component_with_slash);
}

// Construit le chemin final à partir des composants normalisés
static char	*build_normalized_path(char **components, int size)
{
	char	*new_path;
	int		i;

	new_path = ft_strdup("/");
	if (!new_path)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_path = append_path_component(new_path, components[i]);
		if (!new_path)
			return (NULL);
		i++;
	}
	return (new_path);
}

// Traite un chemin relatif avec seulement 5 variables
char	*process_relative_path(char *old_pwd, char *path)
{
	char	*temp_path;
	char	**components;
	char	**result_components;
	int		result_size;
	char	*new_path;

	temp_path = build_full_path(old_pwd, path);
	if (!temp_path)
		return (NULL);
	components = ft_split(temp_path, '/');
	free(temp_path);
	if (!components)
		return (NULL);
	result_components = allocate_result_array(count_array_elements(components));
	if (!result_components)
		return (free_array(components), NULL);
	result_size = 0;
	process_path_components(components, &result_components, &result_size);
	new_path = build_normalized_path(result_components, result_size);
	while (result_size > 0)
	{
		result_size--;
		free(result_components[result_size]);
	}
	return (free(result_components), free_array(components), new_path);
}
