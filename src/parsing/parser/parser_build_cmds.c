/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build_cmds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 15:45:22 by lundaevv          #+#    #+#             */
/*   Updated: 2025/12/17 23:29:49 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*skip_to_next_command(t_token *cur)
{
	while (cur && cur->type != TOKEN_PIPE)
		cur = cur->next;
	if (cur && cur->type == TOKEN_PIPE)
		cur = cur->next;
	return (cur);
}

/*
** Fill argv for each command in the pipeline.
** Returns 0 on success, 1 on error.
*/
int	build_pipeline_cmds(t_pipeline *p, t_token *tokens)
{
	t_token	*cur;
	int		i;

	cur = tokens;
	i = 0;
	while (i < p->cmd_count)
	{
		if (!cur)
			return (1);
		p->cmds[i].argv = build_argv_simple(cur);
		if (!p->cmds[i].argv)
			return (1);
		p->cmds[i].redirs = build_redirs_simple(cur, &p->cmds[i].redir_count);
		if (count_redirs_simple(cur) > 0 && !p->cmds[i].redirs)
			return (1);
		cur = skip_to_next_command(cur);
		i++;
	}
	return (0);
}
