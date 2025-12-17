/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 20:36:12 by lundaevv          #+#    #+#             */
/*   Updated: 2025/12/17 23:31:52 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*token_name(t_token *token)
{
	if (!token)
		return ("newline");
	if (token->type == TOKEN_PIPE)
		return ("|");
	if (token->type == TOKEN_REDIR_IN)
		return ("<");
	if (token->type == TOKEN_REDIR_OUT)
		return (">");
	if (token->type == TOKEN_REDIR_APPEND)
		return (">>");
	if (token->type == TOKEN_HEREDOC)
		return ("<<");
	return (token->value);
}

static int	syntax_error(t_token *near)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd((char *)token_name(near), 2);
	ft_putendl_fd("'", 2);
	return (1);
}

static int	need_word_after_redir(t_token *token)
{
	if (!is_redir_token(token->type))
		return (0);
	if (!token->next || token->next->type != TOKEN_WORD)
		return (1);
	return (0);
}

int	validate_syntax(t_token *token)
{
	t_token	*prev;

	if (!token)
		return (0);
	if (token->type == TOKEN_PIPE)
		return (syntax_error(token));
	prev = NULL;
	while (token)
	{
		if (token->type == TOKEN_PIPE && (!prev || prev->type == TOKEN_PIPE))
			return (syntax_error(token));
		if (need_word_after_redir(token))
			return (syntax_error(token->next));
		prev = token;
		token = token->next;
	}
	if (prev && prev->type == TOKEN_PIPE)
		return (syntax_error(NULL));
	return (0);
}
