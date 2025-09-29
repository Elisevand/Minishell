/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:33:53 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/28 16:04:35 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_argument_valid(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
	{
		printf("minishell: export: `%s': not a valid identifier\n", str);
		return (0);
	}
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '+' )
		{
			printf("minishell: export: `%s': not a valid identifier\n", str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	write_export(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] != '\0')
			return (0);
		i++;
	}
	return (1);
}

int	check_export_args(t_command *cmd)
{
	return (!cmd || !cmd->args || !cmd->args[1] || write_export(cmd));
}

int	process_export_arg(t_shell *shell, char *arg)
{
	if (arg[0] == '\0')
		return (0);
	if (!is_argument_valid(arg))
		return (1);
	if (!handle_append(shell, arg))
		handle_assignment(shell, arg);
	return (0);
}

int	ft_export(t_command *cmd, t_shell *shell)
{
	int	i;
	int	error_flag;

	if (check_export_args(cmd))
		return (handle_no_args(shell));
	error_flag = 0;
	i = 1;
	while (cmd->args[i])
	{
		error_flag |= process_export_arg(shell, cmd->args[i]);
		i++;
	}
	return (error_flag);
}
