/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboland <rboland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:13:42 by rboland           #+#    #+#             */
/*   Updated: 2025/03/24 15:13:42 by rboland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_prompt(void)
{
	char	*username;
	char	hostname[256];
	char	cwd[PATH_MAX];
	char	*prompt;

	username = getenv("USER");
	if (!username)
		username = "user";
	if (gethostname(hostname, sizeof(hostname)) != 0)
		ft_strlcpy(hostname, "localhost", sizeof(hostname));
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		ft_strlcpy(cwd, "~", sizeof(cwd));
	prompt = malloc(ft_strlen(username) + ft_strlen(hostname)
			+ ft_strlen(cwd) + 50);
	if (!prompt)
		return (ft_strdup("minishell$ "));
	sprintf(prompt, "\033[1;36m%s@%s:%s$\033[0m ", username, hostname, cwd);
	return (prompt);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] > s2[i] || s1[i] < s2[i])
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (s1[i] - s2[i]);
}

int	has_unclosed_quotes(char *input)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	return (in_single_quotes || in_double_quotes);
}
