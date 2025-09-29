/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:24:55 by rboland           #+#    #+#             */
/*   Updated: 2025/03/18 13:24:55 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Initialize shell structure */
static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env(envp);
	shell->last_exit_status = 0;
	setup_signals();
	printf("	\033[33mBienvenue dans minishell\033[33m\n");
}

/* Handle shell input */
static void	handle_input(char *input, t_shell *shell)
{
	if (input[0] != '\0')
	{
		add_history(input);
		process_input(input, shell);
	}
}

/* Process command input loop */
static void	command_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;

	while (1)
	{
		g_signal = 0;
		prompt = get_prompt();
		input = readline(prompt);
		if (!input)
		{
			write(STDOUT_FILENO, "\n", 1);
			write(STDOUT_FILENO, "exit\n", 5);
			free(prompt);
			break ;
		}
		handle_input(input, shell);
		free(prompt);
		free(input);
	}
}

/* Check command line arguments */
static int	check_args(int argc, char **argv)
{
	if (argc != 1)
	{
		printf("minishell: %s: Unexpected argument\n", argv[1]);
		return (0);
	}
	return (1);
}

/* Main function */
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (!check_args(argc, argv))
		return (1);
	init_shell(&shell, envp);
	command_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	return (shell.last_exit_status);
}
