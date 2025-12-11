/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 19:31:31 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 19:55:20 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* Return 1 if the line should be treated as an "exit" command.
* For now we handle:
*   - "exit"
*   - "exit" followed by a space and anything else.
*/
int	is_exit_command(const char *line)
{
	if (!line || line[0] == '\0')
		return (0);
	if (ft_strncmp(line, "exit", 4) != 0)
		return (0);
	if (line[4] != '\0' && line[4] != ' ')
		return (0);
	return (1);
}

/*
** Return 1 if we should exit the shell after this line.
** Also handles history and freeing line in that case.
*/
int	handle_history_and_exit(t_shell *shell, char *line)
{
	if (line[0] != '\0')
		add_history(line);
	if (is_exit_command(line))
	{
		free(line);
		shell->exit_status = 0;
		return (1);
	}
	return (0);
}

/*
** Common cleanup path when expand_tokens fails.
*/
int	handle_expand_error(t_token **tokens, char *line)
{
	if (tokens && *tokens)
		token_list_clear(tokens);
	free(line);
	return (0);
}
