/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:15:04 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 16:45:57 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Gère le cas où le chemin est home
static char	*handle_home_path(t_shell *shell)
{
	char	*home_value;
	char	*path;

	home_value = get_env_value(shell->env, "HOME");
	if (!home_value)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	path = ft_strdup(home_value);
	return (path);
}

// Gère le cas où le chemin commence par ~/ (tilde suivi d'un chemin)
static char	*handle_tilde_path(char *arg, t_shell *shell)
{
	char	*subpath;
	char	*path;

	subpath = ft_substr(arg, 1, ft_strlen(arg) - 1);
	path = ft_strjoin(get_env_value(shell->env, "HOME"), subpath);
	free(subpath);
	if (!path)
		return (ft_putendl_fd("minishell: cd: HOME not set",
				STDERR_FILENO), NULL);
	return (path);
}

// Gère le cas où on veut retourner au répertoire précédent (-)
static char	*handle_minus_path(t_shell *shell)
{
	char	*old_path;

	old_path = get_env_value(shell->env, "OLDPWD");
	if (!old_path)
		return (ft_putendl_fd("minishell: cd: OLDPWD not set",
				STDERR_FILENO), NULL);
	ft_putendl_fd(old_path, STDOUT_FILENO);
	return (ft_strdup(old_path));
}

// Détermine le chemin à utiliser pour la commande cd
char	*set_path(t_command *cmd, t_shell *shell)
{
	if (!cmd->args[1]
		|| ft_strncmp(cmd->args[1], "~", 2) == 0
		|| ft_strncmp(cmd->args[1], "--", 3) == 0)
		return (handle_home_path(shell));
	if (cmd->args[1][0] == '~' && cmd->args[1][1] == '/')
		return (handle_tilde_path(cmd->args[1], shell));
	if (ft_strncmp(cmd->args[1], "-", 2) == 0)
		return (handle_minus_path(shell));
	return (ft_strdup(cmd->args[1]));
}
