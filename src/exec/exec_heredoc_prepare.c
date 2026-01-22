/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_prepare.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:52:45 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 18:54:11 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_heredocs(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	fd;

	if (!cmd || cmd->redir_count <= 0 || !cmd->redirs)
		return (0);
	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_HEREDOC)
		{
			fd = heredoc_open(sh, cmd->redirs[i].target,
					cmd->redirs[i].heredoc_expand);
			if (fd == -130)
				return (130);
			if (fd < 0)
				return (1);
			cmd->redirs[i].hd_fd = fd;
		}
		i++;
	}
	return (0);
}
