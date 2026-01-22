/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_build.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:46:26 by lundaevv          #+#    #+#             */
/*   Updated: 2026/01/22 19:28:44 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	free_redirs_partial(t_redir *r, int filled)
{
	while (filled > 0)
	{
		filled--;
		free(r[filled].target);
	}
	free(r);
	return (1);
}

static void	set_heredoc_expand(t_redir *r, int k, t_token *t)
{
	r[k].heredoc_expand = true;
	if (t && t->next && t->next->has_quotes)
		r[k].heredoc_expand = false;
}

static int	add_one_redir(t_redir *r, int *k, t_token *t)
{
	if (!t || !t->next || t->next->type != TOKEN_WORD)
		return (1);
	if (token_to_redir_type(t->type, &r[*k].type) != 0)
		return (1);
	if (t->type == TOKEN_HEREDOC)
		r[*k].target = ms_unquote_limiter(t->next->value);
	else
		r[*k].target = ft_strdup(t->next->value);
	if (!r[*k].target)
		return (1);
	if (t->type == TOKEN_HEREDOC)
		set_heredoc_expand(r, *k, t);
	else
		r[*k].heredoc_expand = false;
	(*k)++;
	return (0);
}

static int	fill_redirs_simple(t_redir *r, t_token *t)
{
	int	k;

	k = 0;
	while (t && t->type != TOKEN_PIPE)
	{
		if (is_redir_token(t->type))
		{
			if (add_one_redir(r, &k, t) != 0)
				return (free_redirs_partial(r, k));
			t = t->next->next;
			continue ;
		}
		t = t->next;
	}
	return (0);
}

t_redir	*build_redirs_simple(t_token *tokens, int *out_count)
{
	t_redir	*r;
	int		count;

	if (!out_count)
		return (NULL);
	count = count_redirs_simple(tokens);
	if (count == 0)
	{
		*out_count = 0;
		return (NULL);
	}
	r = (t_redir *)malloc(sizeof(t_redir) * (size_t)count);
	if (!r)
	{
		*out_count = 0;
		return (NULL);
	}
	init_redirs(r, count);
	if (fill_redirs_simple(r, tokens) != 0)
	{
		*out_count = 0;
		return (NULL);
	}
	*out_count = count;
	return (r);
}
