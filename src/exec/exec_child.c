/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:07:47 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 15:51:13 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	connect_pipes(t_pipeline *p, int i, int pipes[][2])
{
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
			return (perror("dup2"), exit(1));
	}
	if (i < p->cmd_count - 1)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
			return (perror("dup2"), exit(1));
	}
}

static void	close_pipes(t_pipeline *p, int pipes[][2])
{
	int	k;
	int	pipe_count;

	pipe_count = p->cmd_count - 1;
	k = 0;
	while (k < pipe_count)
	{
		close(pipes[k][0]);
		close(pipes[k][1]);
		k++;
	}
}

static void	run_cmd_in_child(t_shell *sh, t_cmd *cmd)
{
	if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
		exit(run_builtin(sh, cmd));
	exec_cmd(cmd, sh->envp);
}

void	child_run_pipeline_cmd(t_shell *sh, t_pipeline *p, int i,
		int pipes[][2])
{
	t_cmd	*cmd;
	int		r;

	signals_child_exec();
	cmd = &p->cmds[i];
	if (ms_cmd_is_empty(cmd))
		exit(0);
	connect_pipes(p, i, pipes);
	close_pipes(p, pipes);
	r = apply_redirections(sh, cmd);
	if (r != 0)
		exit(r);
	if (ms_cmd_is_empty(cmd))
		exit(0);
	run_cmd_in_child(sh, cmd);
}
