/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_oldpwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:11:34 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/25 13:16:33 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	update_old_pwd(char *old_pwd, t_shell *shell)
{
	char	*pwd_env;

	if (getcwd(old_pwd, PATH_MAX) != NULL)
		return (1);
	ft_putstr_fd("minishell: cd: error retrieving current ", STDERR_FILENO);
	ft_putstr_fd("directory: getcwd: ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	pwd_env = get_env_value(shell->env, "PWD");
	if (pwd_env)
	{
		ft_strlcpy(old_pwd, pwd_env, PATH_MAX);
		return (1);
	}
	return (error_cd("minishell: cd: error retrieving current directory"));
}

// Fonction principale pour gérer l'échec de PWD
void	handle_pwd_failure(char *path, char *old_pwd, t_shell *shell)
{
	char	*new_path;

	if (path[0] == '/')
		new_path = ft_strdup(path);
	else
		new_path = process_relative_path(old_pwd, path);
	if (new_path)
	{
		set_env_value(shell->env, "PWD", new_path);
		free(new_path);
	}
}
