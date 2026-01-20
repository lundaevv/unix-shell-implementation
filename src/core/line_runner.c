/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_runner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:01:55 by lundaevv          #+#    #+#             */
/*   Updated: 2026/01/20 15:45:31 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cut_first_newline(char *line)
{
	char	*nl;

	if (!line)
		return ;
	nl = ft_strchr(line, '\n');
	if (nl)
		*nl = '\0';
}

static void	cleanup_line(char *line, t_token **tokens, t_pipeline **p)
{
	if (p && *p)
		free_pipeline(*p);
	if (tokens && *tokens)
		token_list_clear(tokens);
	if (line)
		free(line);
}

int	run_line(t_shell *shell, char *line)
{
	t_token		*tokens;
	t_pipeline	*p;
	int			rc;

	tokens = NULL;
	p = NULL;
	rc = 0;
	cut_first_newline(line);
	if (is_only_spaces(line))
		return (free(line), 0);
	if (handle_history_and_exit(shell, line))
		return (free(line), 1);
	if (handle_unclosed_quotes(shell, line))
		return (free(line), 0);
	rc = line_build_state(shell, line, &tokens, &p);
	if (rc != 0)
		return (cleanup_line(line, &tokens, &p), 0);
	cleanup_line(line, &tokens, &p);
	return (0);
}
