/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 19:31:31 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/21 14:49:22 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_only_spaces(const char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/*
** Return 1 if we should exit the shell after this line.
** Also handles history and freeing line in that case.
*/
int	handle_history_and_exit(t_shell *shell, char *line)
{
	(void)shell;
	if (!line || is_only_spaces(line))
		return (0);
	add_history(line);
	return (0);
}

/*
** Common cleanup path when expand_tokens fails.
*/
int	handle_expand_error(t_token **tokens, char *line)
{
	(void)line;
	if (tokens && *tokens)
		token_list_clear(tokens);
	return (2);
}
