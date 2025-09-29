/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:47:27 by rboland           #+#    #+#             */
/*   Updated: 2025/04/25 09:47:27 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Update input file descriptor if needed */
int	update_input_fd(t_command *cmd, int in_fd, int heredoc_fd)
{
	int				has_input_redirection;
	t_redirection	*redir;

	has_input_redirection = 0;
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN)
		{
			has_input_redirection = 1;
			break ;
		}
		redir = redir->next;
	}
	if (!has_input_redirection && heredoc_fd != STDIN_FILENO)
		return (heredoc_fd);
	return (in_fd);
}

/* Execute builtin with redirections */
int	execute_builtin_with_redirects(t_command *cmd, int in_fd,
			int out_fd, t_shell *shell)
{
	int	stdin_copy;
	int	stdout_copy;
	int	status;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (!setup_redirections(cmd, in_fd, out_fd))
	{
		restore_std_fds(stdin_copy, stdout_copy);
		return (1);
	}
	status = execute_builtin(cmd, shell);
	restore_std_fds(stdin_copy, stdout_copy);
	return (status);
}

/* Prepare to execute external command */
static void	prepare_external_cmd(t_command *cmd, int heredoc_fd)
{
	if (cmd->args[0] == NULL)
	{
		if (heredoc_fd != STDIN_FILENO)
			close(heredoc_fd);
		exit(0);
	}
}

/* Execute external command */
int	execute_external_command(t_command *cmd, t_shell *shell,
		int fds[2], int heredoc_fd)
{
	char					*exec_path;
	pid_t					pid;
	t_child_process_ctx		ctx;

	exec_path = find_executable(cmd->args[0], shell);
	if (!exec_path)
	{
		print_cmd_not_found(cmd->args[0]);
		if (heredoc_fd != STDIN_FILENO)
			close(heredoc_fd);
		return (127);
	}
	pid = fork();
	if (pid < 0)
		return (handle_fork_error(exec_path, heredoc_fd));
	if (pid == 0)
	{
		prepare_external_cmd(cmd, heredoc_fd);
		ctx = init_child_process_ctx(cmd, fds, exec_path, shell);
		child_process(cmd, &ctx);
	}
	return (parent_process(pid, exec_path, cmd, heredoc_fd));
}

/* Set up redirections for a command */
int	setup_redirections(t_command *cmd, int in_fd, int out_fd)
{
	if (!cmd)
		return (0);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	return (setup_cmd_redirections(cmd));
}
