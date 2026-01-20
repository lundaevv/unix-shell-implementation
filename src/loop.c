/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:12:40 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/20 15:46:33 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Readline:
** - Prints the prompt
** - Returns the line (malloc'ed at the &line)
** 			- or NULL (if only EOF (Ctrl+D) && empty line)
** - If NULL - prints exit and returns NULL
*/
static char	*read_line_with_prompt(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (!line)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		return (NULL);
	}
	return (line);
}

/*
** Main shell loop:
** - Gets the input line (if NULL - stops)
** - Starts to process it
*/
void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = read_line_with_prompt();
		if (!line)
			break ;
		if (run_line(shell, line))
			break ;
	}
}
