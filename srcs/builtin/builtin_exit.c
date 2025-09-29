/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:36:16 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/25 15:36:22 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_piped(t_command *cmd)
{
	t_token	*current;

	current = cmd->token;
	while (current)
	{
		if (current->type == 1)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	is_arg_valid(char *arg)
{
	int		i;

	i = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			return (2);
		}
		i++;
	}
	return (0);
}

// Exit the shell
int	ft_exit(t_command *cmd, t_shell *shell)
{
	int	exit_code;
	int	arg_check;

	arg_check = 0;
	exit_code = 0;
	if (!cmd || !cmd->args)
		return (1);
	if (!is_piped(cmd))
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (cmd->args[1])
	{
		arg_check = is_arg_valid(cmd->args[1]);
		if (arg_check == 2)
			exit_code = 2;
		else if (cmd->args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	free_env(shell->env);
	rl_clear_history();
	exit(exit_code);
}
