/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evan-dro <evan-dro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:36:55 by evan-dro          #+#    #+#             */
/*   Updated: 2025/04/25 15:40:13 by evan-dro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_args(t_command *cmd)
{
	int	count;

	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	return (count);
}

static int	allocate_new_arrays(int count,
	char ***new_args, int **new_arg_quoted)
{
	*new_args = malloc(sizeof(char *) * (count + 2));
	*new_arg_quoted = malloc(sizeof(int) * (count + 2));
	if (!(*new_args) || !(*new_arg_quoted))
	{
		if (*new_args)
			free(*new_args);
		if (*new_arg_quoted)
			free(*new_arg_quoted);
		return (0);
	}
	return (1);
}

static void	copy_existing_args(t_command *cmd, char **new_args,
	int *new_arg_quoted, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		new_arg_quoted[i] = cmd->arg_quoted[i];
		i++;
	}
}

static int	add_new_arg(char **new_args, char *arg, int count)
{
	new_args[count] = ft_strdup(arg);
	if (!new_args[count])
		return (0);
	return (1);
}

// Add word to command arguments
void	add_arg(t_command *cmd, char *arg, int quoted_state)
{
	char	**new_args;
	int		*new_arg_quoted;
	int		count;

	if (!cmd || !arg)
		return ;
	count = count_args(cmd);
	if (!allocate_new_arrays(count, &new_args, &new_arg_quoted))
		return ;
	copy_existing_args(cmd, new_args, new_arg_quoted, count);
	if (!add_new_arg(new_args, arg, count))
	{
		free(new_args);
		free(new_arg_quoted);
		return ;
	}
	new_arg_quoted[count] = quoted_state;
	new_args[count + 1] = NULL;
	new_arg_quoted[count + 1] = 0;
	free(cmd->args);
	free(cmd->arg_quoted);
	cmd->args = new_args;
	cmd->arg_quoted = new_arg_quoted;
}
