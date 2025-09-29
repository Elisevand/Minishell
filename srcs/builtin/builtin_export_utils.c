/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:45:58 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 16:03:27 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Sert a trier l'array d'environnement
void	bubble_sort(char **env_array)
{
	int		len;
	int		i;
	int		j;
	char	*temp;

	len = 0;
	while (env_array[len])
		len++;
	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - 1)
		{
			if (ft_strcmp(env_array[j], env_array[j + 1]) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	handle_no_args(t_shell *shell)
{
	char	**env_array;
	int		i;

	env_array = env_to_array_export(shell->env);
	bubble_sort(env_array);
	i = 0;
	while (env_array[i])
	{
		printf("declare -x %s\n", env_array[i]);
		free(env_array[i]);
		i++;
	}
	free(env_array);
	return (0);
}

int	handle_append(t_shell *shell, char *arg)
{
	char	*plus_equal;
	char	*key;
	char	*to_append;
	char	*existing;
	char	*new_val;

	plus_equal = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (!plus_equal)
		return (0);
	*plus_equal = '\0';
	key = arg;
	to_append = plus_equal + 2;
	existing = get_env_value(shell->env, key);
	if (existing != NULL)
		new_val = ft_strjoin(existing, to_append);
	else
		new_val = ft_strjoin("", to_append);
	set_env_value(shell->env, key, new_val);
	free(new_val);
	*plus_equal = '+';
	return (1);
}

void	handle_assignment(t_shell *shell, char *arg)
{
	char	*equals_sign;

	equals_sign = ft_strchr(arg, '=');
	if (equals_sign)
	{
		*equals_sign = '\0';
		set_env_value(shell->env, arg, equals_sign + 1);
		*equals_sign = '=';
	}
	else
	{
		if (!has_env_key(shell->env, arg))
			set_env_value(shell->env, arg, NULL);
	}
}
