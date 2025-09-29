/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_process_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:47:57 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:47:57 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Setup signals for child process */
void	setup_child_process_signals(t_command *cmd)
{
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "./minishell") == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		setpgid(0, 0);
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
	}
}

/* Execute command in child process */
void	execute_child_command(t_command *cmd, char *exec_path, t_shell *shell)
{
	char	**env_array;

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

/* Handle parent process */
int	parent_process(pid_t pid, char *exec_path, t_command *cmd, int heredoc_fd)
{
	int	status;

	free(exec_path);
	handle_minishell_parent(cmd, pid);
	if (heredoc_fd != STDIN_FILENO)
		close(heredoc_fd);
	waitpid(pid, &status, 0);
	restore_after_minishell(cmd);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/* Special handling for nested minishell parent */
void	handle_minishell_parent(t_command *cmd, pid_t pid)
{
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "./minishell") == 0)
	{
		setpgid(pid, pid);
		tcsetpgrp(STDIN_FILENO, pid);
	}
}

/* Restore terminal control after minishell */
void	restore_after_minishell(t_command *cmd)
{
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "./minishell") == 0)
		tcsetpgrp(STDIN_FILENO, getpgrp());
}
