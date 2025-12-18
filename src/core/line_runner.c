/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_runner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:01:55 by lundaevv          #+#    #+#             */
/*   Updated: 2025/12/18 14:48:48 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_line(char *line, t_token **tokens, t_pipeline **p)
{
	if (p && *p)
		free_pipeline(*p);
	if (tokens && *tokens)
		token_list_clear(tokens);
	free(line);
}

int	run_line(t_shell *shell, char *line)
{
	t_token		*tokens;
	t_pipeline	*p;
	int			rc;

	tokens = NULL;
	p = NULL;
	if (handle_history_and_exit(shell, line))
		return (1);
	if (handle_unclosed_quotes(shell, line))
		return (0);
	rc = line_build_state(shell, line, &tokens, &p);
	if (rc != 0)
		return (0);
	cleanup_line(line, &tokens, &p);
	return (0);
}
