/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_multi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:08:00 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 15:51:37 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_pipeline_prepare(t_shell *sh, t_pipeline *p, int *pipe_count)
{
	if (!p || p->cmd_count <= 0)
		return (sh->exit_status = 0);
	if (p->cmd_count == 1)
		return (exec_pipeline_single(sh, &p->cmds[0]));
	*pipe_count = p->cmd_count - 1;
	if (*pipe_count > 1024)
		return (sh->exit_status = 1);
	return (0);
}

static int	ms_fork_all(t_shell *sh, t_pipeline *p,
			int pipes[][2], pid_t *pids)
{
	int	i;

	i = 0;
	while (i < p->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (perror("fork"), 1);
		if (pids[i] == 0)
			child_run_pipeline_cmd(sh, p, i, pipes);
		i++;
	}
	return (0);
}

int	exec_pipeline_multi(t_shell *sh, t_pipeline *p)
{
	int		pipe_count;
	int		pipes[1024][2];
	pid_t	pids[1024];

	if (ms_pipeline_prepare(sh, p, &pipe_count) != 0)
		return (sh->exit_status);
	if (ms_make_pipes(pipes, pipe_count) != 0)
		return (sh->exit_status = 1);
	signals_parent_exec();
	if (ms_fork_all(sh, p, pipes, pids) != 0)
		return (ms_close_all_pipes(pipes, pipe_count), sh->exit_status = 1);
	ms_close_all_pipes(pipes, pipe_count);
	sh->exit_status = ms_wait_last(pids, p->cmd_count);
	signals_interactive();
	return (sh->exit_status);
}
