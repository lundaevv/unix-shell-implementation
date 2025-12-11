/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 19:18:50 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 19:56:56 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Debug print of tokens
*/
void	token_list_print(t_token *list)
{
	while (list)
	{
		if (list->type == TOKEN_WORD)
			ft_printf("WORD					: '%s'\n", list->value);
		else if (list->type == TOKEN_PIPE)
			ft_printf("PIPE					: '%s'\n", list->value);
		else if (list->type == TOKEN_REDIR_IN)
			ft_printf("REDIR_IN				: '%s'\n", list->value);
		else if (list->type == TOKEN_REDIR_OUT)
			ft_printf("REDIR_OUT				: '%s'\n", list->value);
		else if (list->type == TOKEN_REDIR_APPEND)
			ft_printf("REDIR_APPEND				: '%s'\n", list->value);
		else if (list->type == TOKEN_HEREDOC)
			ft_printf("HEREDOC					: '%s'\n", list->value);
		list = list->next;
	}
}

static void	ms_debug_line(char *line)
{
	ft_putstr_fd("You typed: ", STDOUT_FILENO);
	ft_putendl_fd(line, STDOUT_FILENO);
}

static void	ms_debug_tokens(t_token *tokens)
{
	if (!tokens)
		return ;
	token_list_print(tokens);
}

/*
** Print argv for each command in the pipeline (if present).
*/
static void	ms_debug_pipeline(t_pipeline *p)
{
	int	i;
	int	j;

	if (!p || p->cmd_count <= 0)
		return ;
	j = 0;
	while (j < p->cmd_count)
	{
		ft_printf("CMD[%d] argv:\n", j);
		if (p->cmds[j].argv)
		{
			i = 0;
			while (p->cmds[j].argv[i])
			{
				ft_printf("  argv[%d] = '%s'\n", i, p->cmds[j].argv[i]);
				i++;
			}
		}
		j++;
	}
}

/*
** Global debug entry point. Call this once per line.
*/
void	ms_debug_state(t_shell *shell, char *line,
			t_token *tokens, t_pipeline *p)
{
	(void)shell; // позже можно печатать exit_status, env и т.п.
	if (!line)
		return ;
	ms_debug_line(line);
	ms_debug_tokens(tokens);
	ms_debug_pipeline(p);
}
