/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:43:13 by rboland           #+#    #+#             */
/*   Updated: 2025/04/28 16:43:13 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Handle command heredoc */
int	handle_command_heredoc(t_command *cmd, t_shell *shell)
{
	int	heredoc_fd;

	heredoc_fd = STDIN_FILENO;
	if (cmd->has_heredoc)
	{
		heredoc_fd = setup_heredoc(cmd, shell);
		if (heredoc_fd < 0)
			return (-1);
	}
	return (heredoc_fd);
}

/* Execute a command */
int	execute_command(t_command *cmd, int in_fd, int out_fd, t_shell *shell)
{
	int		heredoc_fd;
	int		fds[2];

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	heredoc_fd = handle_command_heredoc(cmd, shell);
	if (heredoc_fd < 0)
		return (1);
	in_fd = update_input_fd(cmd, in_fd, heredoc_fd);
	fds[0] = in_fd;
	fds[1] = out_fd;
	if (is_builtin(cmd->args[0]))
		return (execute_builtin_with_redirects(cmd, in_fd, out_fd, shell));
	else
		return (execute_external_command(cmd, shell, fds, heredoc_fd));
}
