/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argv_count.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:47:22 by lundaevv          #+#    #+#             */
/*   Updated: 2025/12/17 23:35:04 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words_no_redirs(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redir_token(tokens->type))
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
				tokens = tokens->next;
			continue ;
		}
		if (tokens->type == TOKEN_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}
