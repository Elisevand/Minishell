/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:00:58 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 14:00:58 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Free paths array */
void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

/* Handle pipe creation error */
int	handle_pipe_error(int *heredoc_fds, int cmd_count)
{
	perror("pipe");
	cleanup_heredocs(heredoc_fds, cmd_count);
	return (1);
}

/* Execute pipeline command child process */
void	execute_pipeline_command_child(t_pipeline *pipeline, t_shell *shell,
		int i, t_child_ctx *ctx)
{
	t_command	*cmd;

	cmd = pipeline->commands[i];
	if (cmd->has_heredoc && ctx->heredoc_fds[i] != STDIN_FILENO)
	{
		dup2(ctx->heredoc_fds[i], STDIN_FILENO);
		close(ctx->heredoc_fds[i]);
	}
	close_other_heredocs(pipeline, i, ctx->heredoc_fds);
	free(ctx->heredoc_fds);
	if (!setup_cmd_redirections(cmd))
		exit(1);
	execute_command_by_type(cmd, shell);
	exit(1);
}

/* Execute command by type (builtin or external) */
void	execute_command_by_type(t_command *cmd, t_shell *shell)
{
	char	*exec_path;
	char	**env_array;

	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	else
	{
		exec_path = find_executable(cmd->args[0], shell);
		if (!exec_path)
		{
			print_cmd_not_found(cmd->args[0]);
			exit(127);
		}
		env_array = env_to_array(shell->env);
		if (!env_array)
		{
			free(exec_path);
			exit(1);
		}
		execve(exec_path, cmd->args, env_array);
		perror(exec_path);
		free(exec_path);
		free_env_array(env_array);
		exit(1);
	}
}

/* Print command not found error */
void	print_cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putendl_fd(cmd, 2);
}
