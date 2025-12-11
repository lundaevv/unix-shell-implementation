/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:12:40 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 19:35:46 by vlundaev         ###   ########.fr       */
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
** Process a single input line:
** - history + exit
** - lexer
** - expander
** - parser
** - debug
*/
static int	process_line(t_shell *shell, char *line)
{
	t_token		*tokens;
	t_pipeline	*p;

	if (handle_history_and_exit(shell, line))
		return (1);
	tokens = lexer_tokenize(line);
	if (!tokens)
	{
		free(line);
		return (0);
	}
	if (expand_tokens(tokens, shell->env, shell->exit_status) != 0)
		return (handle_expand_error(&tokens, line));
	p = parse_pipeline(tokens);
	ms_debug_state(shell, line, tokens, p);
	if (p)
		free_pipeline(p);
	token_list_clear(&tokens);
	free(line);
	return (0);
}

/*
** Main shell loop:
** - Gets the input line (if NULL - stops)
** - Starts to process it
*/
void	shell_loop(t_shell *shell)
{
	char	*line;

	// line = NULL;
	while (1)
	{
		line = read_line_with_prompt();
		if (!line)
			break ;
		if (process_line(shell, line)) // can be freed here instead of the function
			break ;
	}
}
