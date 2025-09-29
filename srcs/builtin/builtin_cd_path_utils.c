/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_path_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:17:16 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/25 13:28:49 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Fonction pour libérer un tableau de chaînes
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

// Fonction pour compter les éléments d'un tableau
int	count_array_elements(char **array)
{
	int	count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}

// Traite un composant du chemin et met à jour le résultat
static void	process_path_component(char *component,
	char ***result, int *result_size)
{
	if (ft_strcmp(component, ".") == 0 || ft_strlen(component) == 0)
		return ;
	if (ft_strcmp(component, "..") == 0)
	{
		if (*result_size > 0)
			(*result_size)--;
		return ;
	}
	(*result)[*result_size] = ft_strdup(component);
	(*result_size)++;
}

// Traite les composants du chemin et construit un tableau normalisé
void	process_path_components(char **components,
	char ***result, int *result_size)
{
	int	i;

	*result_size = 0;
	i = 0;
	while (components[i])
	{
		process_path_component(components[i], result, result_size);
		i++;
	}
}

// Alloue de la mémoire pour le tableau de résultats
char	**allocate_result_array(int size)
{
	char	**result;

	result = (char **)malloc(sizeof(char *) * (size + 1));
	if (!result)
		return (NULL);
	result[size] = NULL;
	return (result);
}
