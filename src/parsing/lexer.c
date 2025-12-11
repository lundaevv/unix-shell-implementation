/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:20:30 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 17:15:10 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Very simple lexer:
** - skips spaces
** - collects characters until next space
** - creates a TOKEN_WORD
*/
t_token	*lexer_tokenize(const char *line)
{
	t_token	*tokens;
	t_token	*new_token;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		while (line[i] != '\0' && is_space(line[i]))
			i++;
		if (line[i] == '\0')
			break ;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			new_token = lexer_read_operator(line, &i);
		else
			new_token = lexer_read_word(line, &i);
		if (!new_token)
		{
			token_list_clear(&tokens);
			return (NULL);
		}
		token_add_back(&tokens, new_token);
	}
	return (tokens);
}
