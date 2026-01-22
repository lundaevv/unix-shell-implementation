/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:12:40 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 19:26:45 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_line_with_prompt(void)
{
	char	*line;
	char	*clean;

	if (isatty(STDIN_FILENO))
	{
		signals_interactive();
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			return (NULL);
		}
		return (line);
	}
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	clean = ft_strtrim(line, "\n");
	free(line);
	return (clean);
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (!shell->should_exit)
	{
		line = read_line_with_prompt();
		if (!line)
			break ;
		run_line(shell, line);
	}
}
