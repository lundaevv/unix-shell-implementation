/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_count.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:31:17 by lundaevv          #+#    #+#             */
/*   Updated: 2025/12/17 23:29:06 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_redirs_simple(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_REDIR_APPEND
			|| tokens->type == TOKEN_HEREDOC)
		{
			if (tokens->next && tokens->next->type == TOKEN_WORD)
				count++;
		}
		tokens = tokens->next;
	}
	return (count);
}
