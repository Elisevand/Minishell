/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:20:17 by rboland           #+#    #+#             */
/*   Updated: 2025/03/18 13:20:17 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structure.h"
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

// Token types
# define TOKEN_WORD      0  // Regular word/command/argument
# define TOKEN_PIPE      1  // |
# define TOKEN_REDIR_IN  2  // <
# define TOKEN_REDIR_OUT 3  // >
# define TOKEN_APPEND    4  // >>
# define TOKEN_HEREDOC   5  // <<

// Exit code
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_INVALID_USAGE 2
# define EXIT_NOT_EXECUTABLE 126
# define EXIT_NOT_FOUND 127

// Global signal variable
extern volatile sig_atomic_t	g_signal;

/****************************************************************
 * 							UTILS								*
 ****************************************************************/

/* ---- utils.c ---- */
char				*get_prompt(void);
int					ft_strcmp(char *s1, char *s2);
int					has_unclosed_quotes(char *input);

/****************************************************************
 * 							ENVIRONMENT							*
 ****************************************************************/

/* ---- environment.c ---- */
t_env				*init_env(char **envp);
void				free_env(t_env *env);
char				*get_env_value(t_env *env, char *key);
void				set_env_value(t_env *env, char *key, char *value);
void				remove_env_var(t_env *env, char *key);
char				**env_to_array(t_env *env);
char				**env_to_array_export(t_env *env);
int					has_env_key(t_env *env, char *key);
void				free_env_array(char **env_array);
t_env				*create_env_node(char *key, char *value);
void				add_env_node(t_env **env_list,
						t_env **current, t_env *new_node);
int					check_special_env_var(char *entry,
						char *var_name, int name_len);
void				process_env_entry(char *entry,
						t_env **env_list, t_env **current);
int					count_env_vars(t_env *env);
char				*create_env_array_entry(char *key, char *value);
void				cleanup_env_array(char **env_array, int index);
int					count_export_vars(t_env *env);
int					should_skip_env_var(t_env *current);

/****************************************************************
 * 							BUILTIN								*
 ****************************************************************/

int					ft_echo(t_command *cmd);
// for cd
int					error_cd(char *str);
int					ft_cd(t_command *cmd, t_shell *shell);
int					update_old_pwd(char *old_pwd, t_shell *shell);
void				handle_pwd_failure(char *path, char *old_pwd,
						t_shell *shell);
void				free_array(char **array);
int					count_array_elements(char **array);
void				process_path_components(char **components,
						char ***result, int *result_size);
char				**allocate_result_array(int size);
char				*set_path(t_command *cmd, t_shell *shell);
char				*process_relative_path(char *old_pwd, char *path);
int					ft_pwd(void);
// for export
int					ft_export(t_command *cmd, t_shell *shell);
int					handle_no_args(t_shell *shell);
int					handle_append(t_shell *shell, char *arg);
void				handle_assignment(t_shell *shell, char *arg);
int					ft_unset(t_command *cmd, t_shell *shell);
int					ft_env(t_shell *shell);
int					ft_exit(t_command *cmd, t_shell *shell);

/****************************************************************
 * 							TOKENIZER							*
 ****************************************************************/

/* ---- tokenizer.c ---- */
t_token				*new_token(char *value, int type, int quoted_state);
void				add_token(t_token **tokens, t_token *new);
char				*extract_quoted_str(char *input, int *i, char quote_char);

/* ---- tokenizer_utils.c ---- */
void				process_pipe_token(t_token **tokens, int *i);
void				process_input_redir(t_token **tokens, char *input, int *i);
void				process_output_redir(t_token **tokens, char *input, int *i);
void				process_quoted_heredoc(t_token **tokens, char *in, int *i,
						int q_type);
void				process_unquoted_heredoc(t_token **tokens,
						char *input, int *i);

/* ---- tokenizer_heredoc.c ---- */
void				skip_whitespace(char *input, int *i);
void				process_heredoc(t_token **tokens, char *input, int *i);
void				process_word_token(t_token **tokens, char *input, int *i);
void				free_tokens(t_token *tokens);

/* ---- extract_word.c ---- */
char				*extract_word(char *input, int *i);

/* ---- tokenize_input.c ---- */
t_token				*tokenize_input(char *input);

/****************************************************************
 * 							PROCESS								*
 ****************************************************************/

/* ---- process.c ---- */
void				process_input(char *input, t_shell *shell);

/* ---- syntax_validation.c ---- */
int					validate_syntax(t_token *tokens);
int					check_consecutive_redirections(t_token *tokens);

/* ---- validation_utils.c ---- */
int					is_redirection(int token_type);
int					check_pipe_errors(t_token *current, t_token *prev);
void				print_redir_error(int token_type);
int					handle_missing_word_error(t_token *current);

/****************************************************************
 * 							PARSER								*
 ****************************************************************/

void				add_arg(t_command *cmd, char *arg, int quoted_state);
void				add_redirection(t_command *cmd,
						char *file, int type, int quoted_state);
void				free_command(t_command *cmd);
void				free_pipeline(t_pipeline *pipeline);
int					count_commands(t_token *tokens);
t_command			*create_command(t_token *tokens);
t_token				*tokenize_input(char *input);
void				free_tokens(t_token *tokens);
t_pipeline			*parse_tokens(t_token *tokens);
void				free_pipeline(t_pipeline *pipeline);

/****************************************************************
 * 							EXPAND								*
 ****************************************************************/

/* ---- expand_variables.c ---- */
char				*expand_variables(char *str, t_shell *shell);

/* ---- expand_utils.c ---- */
int					is_valid_var_char(char c, int first_char);
char				*extract_var_name(char *str, int *i);
char				*expand_single_var(char *str, int *i, t_shell *shell);
int					is_in_dquotes(char *str, int pos);
int					should_expand_var(char *str, int i);

/* ---- expand_cmd.c ---- */
void				expand_command_args(t_command *cmd, t_shell *shell);
void				expand_redirections(t_command *cmd, t_shell *shell);
void				expand_heredoc_delims(t_command *cmd, t_shell *shell);
void				expand_pipeline(t_pipeline *pipeline, t_shell *shell);
char				*process_dquote_char(char *result, char c, int *i);

/****************************************************************
 * 							SIGNAL								*
 ****************************************************************/

/* ---- signal.c ---- */
void				setup_signals(void);

/****************************************************************
 * 							EXECUTE								*
 ****************************************************************/

/* ---- execute_command.c ---- */
int					handle_command_heredoc(t_command *cmd,
						t_shell *shell);
int					execute_command(t_command *cmd, int in_fd,
						int out_fd, t_shell *shell);

/* ---- execute_core.c ---- */
int					is_builtin(char *cmd);
int					execute_builtin(t_command *cmd, t_shell *shell);
char				*find_executable(char *cmd, t_shell *shell);
char				*search_paths(char **paths, char *cmd);
char				*find_exec_in_path(char *path_dir, char *cmd);

/* ---- execute_error.c ---- */
int					handle_fork_error(char *exec_path, int heredoc_fd);
void				close_unneeded_fds(int in_fd, int out_fd);
void				restore_std_fds(int stdin_copy, int stdout_copy);

/* ---- execute_heredoc_signal.c ---- */
void				handle_heredoc_signals(struct sigaction *old_int,
						struct sigaction *old_quit);
void				setup_child_signals(void);

/* ---- execute_heredoc.c ---- */
int					setup_heredoc(t_command *cmd, t_shell *shell);
int					handle_heredoc_fork_error(int pipe_fd[2],
						struct sigaction *old_int, struct sigaction *old_quit);
int					handle_heredoc_parent(pid_t pid, int pipe_fd[2],
						struct sigaction *old_int, struct sigaction *old_quit);

/* ---- execute_path_utils.c ---- */
void				free_paths(char **paths);
int					handle_pipe_error(int *heredoc_fds, int cmd_count);
void				execute_pipeline_command_child(t_pipeline *pipeline,
						t_shell *shell, int i, t_child_ctx *ctx);
void				execute_command_by_type(t_command *cmd, t_shell *shell);
void				print_cmd_not_found(char *cmd);

/* ---- execute_pipeline_utils.c ---- */
t_child_ctx			init_child_ctx(t_pipeline *pipeline, t_shell *shell,
						int i, t_pipeline_iter *iter);
int					init_pipeline_pipe(t_pipeline *pipeline, int pipefds[2][2]);
int					execute_pipeline_cmd(t_pipeline *pipeline, t_shell *shell,
						int i, t_pipeline_iter *iter);
void				handle_parent_pipes(int i, t_pipeline_iter *iter);
void				close_unused_pipes(t_pipeline *pipeline,
						int i, int active_pipe, int pipefds[2][2]);

/* ---- execute_pipeline_cmd.c ---- */
int					setup_next_pipe(t_pipeline *pipeline,
						int i, int pipefds[2][2], int active_pipe);
int					handle_pipe_iter_error(t_pipeline *pipeline, int i,
						t_pipeline_iter *iter);
int					execute_pipeline_iter(t_pipeline *pipeline,
						t_shell *shell, int i, t_pipeline_iter *iter);

/* ---- execute_pipeline_process.c ---- */
pid_t				create_command_process(t_pipeline *pipeline, t_shell *shell,
						int i, t_pipeline_iter *iter);
void				close_other_heredocs(t_pipeline *pipeline,
						int i, int *heredoc_fds);
void				execute_pipeline_child(t_pipeline *pipeline, t_shell *shell,
						int i, t_child_ctx *ctx);
void				handle_child_redirections(t_pipeline *pipeline, int i,
						int in_fd, int out_fd);
void				setup_command_input(int i, int in_fd, t_pipeline *pipeline);

/* ---- execute_pipeline_process2.c ---- */
void				handle_output_redirections(t_command *cmd, int out_fd);
void				execute_external_command1(t_command *cmd, t_shell *shell);
void				execute_child_process(t_pipeline *pipeline,
						t_shell *shell, int i);
void				wait_for_processes(pid_t *pids, int count);

/* ---- execute_pipeline_redirect.c ---- */
void				setup_pipe_redirects(int in_fd, int out_fd);
int					handle_input_redirections(t_command *cmd);
int					process_out_redir(t_redirection *redir);
int					process_append_redir(t_redirection *redir);

/* ---- execute_pipeline_utils.c ---- */
int					setup_heredocs(t_pipeline *pipeline, t_shell *shell,
						int *heredoc_fds);
void				cleanup_heredocs(int *heredoc_fds, int count);

/* ---- execute_pipeline_wait.c ---- */
int					wait_for_pipeline(pid_t *pids, int cmd_count);

/* ---- execute_pipeline.c ---- */
int					prepare_pipeline_resources(t_pipeline *pipeline,
						pid_t **pids, int **heredoc_fds);
int					is_single_builtin(t_pipeline *pipeline);
int					execute_pipeline(t_pipeline *pipeline, t_shell *shell);
int					execute_pipeline_commands(t_pipeline *pipeline,
						t_shell *shell, pid_t *pids, int *heredoc_fds);

/* ---- execute_process_utils.c ---- */
void				setup_child_process_signals(t_command *cmd);
void				execute_child_command(t_command *cmd, char *exec_path,
						t_shell *shell);
int					parent_process(pid_t pid, char *exec_path, t_command *cmd,
						int heredoc_fd);
void				handle_minishell_parent(t_command *cmd, pid_t pid);
void				restore_after_minishell(t_command *cmd);

/* ---- execute_process.c ---- */
int					setup_cmd_redirections(t_command *cmd);
int					setup_input_redir(t_redirection *redir);
int					setup_output_redir(t_redirection *redir, int mode);
t_child_process_ctx	init_child_process_ctx(t_command *cmd,
						int fds[2], char *exec_path, t_shell *shell);
void				child_process(t_command *cmd, t_child_process_ctx *ctx);

/* ---- execute_redirect.c ---- */
int					update_input_fd(t_command *cmd, int in_fd, int heredoc_fd);
int					execute_builtin_with_redirects(t_command *cmd, int in_fd,
						int out_fd, t_shell *shell);
int					execute_external_command(t_command *cmd, t_shell *shell,
						int fds[2], int heredoc_fd);
int					setup_redirections(t_command *cmd, int in_fd, int out_fd);

/* ---- execute_setup.c ---- */
char				*process_heredoc_content(char *delimiter,
						int quoted, t_shell *shell);
void				process_heredoc_lines(t_command *cmd, int pipe_fd[2],
						t_shell *shell);
char				*expand_heredoc_line(char *line, int quoted,
						t_shell *shell);
char				*append_heredoc_line(char *collected, char *line,
						int quoted, t_shell *shell);

#endif