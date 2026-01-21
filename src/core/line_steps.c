/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_steps.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:06:45 by lundaevv          #+#    #+#             */
/*   Updated: 2026/01/21 15:49:54 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	step_lex(char *line, t_token **tokens)
{
	*tokens = lexer_tokenize(line);
	if (!*tokens)
		return (2);
	return (0);
}

static int	step_expand(t_shell *shell, char *line, t_token **tokens)
{
	if (expand_tokens(*tokens, shell->envp, shell->exit_status) != 0)
		return (handle_expand_error(tokens, line));
	return (0);
}

static int	step_syntax(t_shell *shell, char *line, t_token **tokens)
{
	(void)line;
	if (!validate_syntax(*tokens))
		return (0);
	shell->exit_status = 2;
	token_list_clear(tokens);
	return (2);
}

static void	step_parse(t_token *tokens, t_pipeline **p)
{
	*p = parse_pipeline(tokens);
}

int	line_build_state(t_shell *shell, char *line, t_token **out_tokens,
			t_pipeline **out_p)
{
	t_token		*tokens;
	t_pipeline	*p;
	int			rc;

	*out_tokens = NULL;
	*out_p = NULL;
	rc = step_lex(line, &tokens);
	if (rc != 0)
		return (rc);
	rc = step_expand(shell, line, &tokens);
	if (rc != 0)
		return (rc);
	rc = step_syntax(shell, line, &tokens);
	if (rc != 0)
		return (rc);
	step_parse(tokens, &p);
	*out_tokens = tokens;
	*out_p = p;
	return (0);
}
