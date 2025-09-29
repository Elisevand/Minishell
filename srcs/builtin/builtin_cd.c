/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:31:16 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 16:52:24 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_cd(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	if (ft_strcmp(str, "minishell: cd: invalid option") == 0)
		return (2);
	return (1);
}

// Vérifie si les arguments de cd sont valides
static int	validate_cd_args(t_command *cmd)
{
	int	arg_count;

	if (!cmd || !cmd->args)
		return (1);
	arg_count = 0;
	while (cmd->args[arg_count] != NULL)
		arg_count++;
	if (arg_count > 0 && cmd->args[1] && cmd->args[1][0] == '-'
			&& cmd->args[1][1] != '\0' && cmd->args[1][1] != '-')
		return (error_cd("minishell: cd: invalid option"));
	if (arg_count > 2)
		return (error_cd("minishell: cd: too many arguments"));
	return (0);
}

// Gère l'erreur quand chdir échoue
static int	handle_chdir_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	return (1);
}

// Mise à jour des variables d'environnement après cd
static void	update_env_after_cd(char *path, char *old_pwd, t_shell *shell)
{
	char	new_pwd[PATH_MAX];

	set_env_value(shell->env, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
		set_env_value(shell->env, "PWD", new_pwd);
	else
		handle_pwd_failure(path, old_pwd, shell);
}

// Fonction principale cd
int	ft_cd(t_command *cmd, t_shell *shell)
{
	char	old_pwd[PATH_MAX];
	char	*path;
	int		error_code;

	error_code = validate_cd_args(cmd);
	if (error_code)
		return (error_code);
	if (!update_old_pwd(old_pwd, shell))
		return (1);
	path = set_path(cmd, shell);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		error_code = handle_chdir_error(path);
		return (free(path), error_code);
	}
	update_env_after_cd(path, old_pwd, shell);
	return (free(path), 0);
}
