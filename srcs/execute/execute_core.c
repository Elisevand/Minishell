/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:46:24 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:46:24 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Check if command is a built-in */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/* Execute a built-in command */
int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd));
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd, shell));
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (ft_export(cmd, shell));
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (ft_unset(cmd, shell));
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(shell));
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (ft_exit(cmd, shell));
	return (1);
}

/* Find executable in PATH */
char	*find_executable(char *cmd, t_shell *shell)
{
	char	*path;
	char	**paths;
	char	*exec_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = get_env_value(shell->env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	exec_path = search_paths(paths, cmd);
	free_paths(paths);
	return (exec_path);
}

/* Search executable in path directories */
char	*search_paths(char **paths, char *cmd)
{
	int		i;
	char	*exec_path;

	i = 0;
	exec_path = NULL;
	while (paths[i])
	{
		exec_path = find_exec_in_path(paths[i], cmd);
		if (exec_path)
			break ;
		i++;
	}
	return (exec_path);
}

/* Find executable in a specific path directory */
char	*find_exec_in_path(char *path_dir, char *cmd)
{
	char	*dir_with_slash;
	char	*full_path;

	dir_with_slash = ft_strjoin(path_dir, "/");
	full_path = ft_strjoin(dir_with_slash, cmd);
	free(dir_with_slash);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}
