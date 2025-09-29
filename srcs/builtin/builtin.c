/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:14:41 by rboland           #+#    #+#             */
/*   Updated: 2025/04/28 16:56:24 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_n_flag(char *s)
{
	int	i;

	if (s[0] != '-' || s[1] != 'n')
		return (0);
	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

// Echo command (with -n and -nnnn option support)
int	ft_echo(t_command *cmd)
{
	int	i;
	int	n_flag;

	if (!cmd || !cmd->args)
		return (1);
	i = 1;
	n_flag = 0;
	while (cmd->args[i] && is_valid_n_flag(cmd->args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

// Print working directory command
int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_putendl_fd("minishell: pwd: error retrieving current directory",
			STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	return (0);
}

// Remove a variable from the environment
int	ft_unset(t_command *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	if (!cmd || !cmd->args)
		return (1);
	while (cmd->args[i])
	{
		remove_env_var(shell->env, cmd->args[i]);
		i++;
	}
	return (0);
}

// Print the environment
int	ft_env(t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (current->value != NULL)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(current->value, STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}
