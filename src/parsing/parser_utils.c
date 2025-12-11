/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:19:28 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 19:58:21 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Count how many commands are in the token list
** Comamnds are separated by PIPE tokens
*/
int	count_commands(t_token *tokens)
{
	int	count;

	count = 1;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

int	count_words_simple(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

/*
** Initialize an array of t_cmd structures
*/
void	init_cmd_array(t_cmd *cmds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		cmds[i].argv = NULL;
		cmds[i].redirs = NULL;
		cmds[i].redir_count = 0;
		i++;
	}
}
