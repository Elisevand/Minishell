/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:28:13 by rboland           #+#    #+#             */
/*   Updated: 2025/04/28 18:28:13 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

# include <structure.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <errno.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/includes/libft.h"

// Forward declarations
typedef struct s_token				t_token;
typedef struct s_command			t_command;
typedef struct s_pipeline			t_pipeline;
typedef struct s_env				t_env;
typedef struct s_shell				t_shell;
typedef struct s_redirection		t_redirection;
typedef struct s_pipeline_iter		t_pipeline_iter;
typedef struct s_init_pipeline		t_init_pipeline;
typedef struct s_pipe_error			t_pipe_error;
typedef struct s_child_ctx			t_child_ctx;
typedef struct s_child_process_ctx	t_child_process_ctx;

// Structures
struct s_token
{
	char			*value;
	int				type;
	int				quoted_state;
	struct s_token	*next;
};

struct s_redirection
{
	char					*file;
	int						type;
	int						quoted;
	struct s_redirection	*next;
};

struct s_command
{
	char			**args;
	int				*arg_quoted;
	t_redirection	*redirections;
	char			**heredoc_delims;
	int				heredoc_count;
	int				*heredoc_quoted;
	int				has_heredoc;
	t_token			*token;
};

struct s_pipeline
{
	t_command	**commands;
	int			cmd_count;
};

struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
};

struct s_shell
{
	t_env	*env;
	int		last_exit_status;
};

/* Pipeline iteration data structure */
struct s_pipeline_iter
{
	int			in_fd;
	int			out_fd;
	int			pipefds[2][2];
	int			active_pipe;
	pid_t		*pids;
	int			*heredoc_fds;
	t_pipeline	*pipeline;
};

/* Pipeline initialization context */
struct s_init_pipeline
{
	t_pipeline	*pipeline;
	pid_t		*pids;
	int			*heredoc_fds;
	int			in_fd;
	int			active_pipe;
};

/* Pipeline error context */
struct s_pipe_error
{
	t_pipeline	*pipeline;
	int			i;
	int			in_fd;
	int			*heredoc_fds;
	pid_t		*pids;
	int			active_pipe;
	int			pipefds[2][2];
};

/* Child redirection context */
struct s_child_ctx
{
	t_pipeline	*pipeline;
	t_shell		*shell;
	int			i;
	int			in_fd;
	int			out_fd;
	int			*heredoc_fds;
};

/* Child process context */
struct s_child_process_ctx
{
	t_command	*cmd;
	int			in_fd;
	int			out_fd;
	char		*exec_path;
	t_shell		*shell;
};

#endif