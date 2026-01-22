/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_single.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:08:03 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 15:51:51 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_parent_builtin(t_shell *sh, t_cmd *cmd)
{
	int	in;
	int	out;
	int	r;

	if (cmd->argv && cmd->argv[0] && ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		write(1, "exit\n", 5);
	if (ms_stdio_save(&in, &out))
		return (sh->exit_status = 1);
	r = apply_redirections(sh, cmd);
	if (r == 0)
		r = run_builtin(sh, cmd);
	ms_stdio_restore(in, out);
	return (sh->exit_status = r);
}

static int	child_exec(t_shell *sh, t_cmd *cmd)
{
	int	r;

	signals_child_exec();
	r = apply_redirections(sh, cmd);
	if (r != 0)
		return (sh->exit_status = r);
	if (ms_cmd_is_empty(cmd))
		return (sh->exit_status = 0);
	if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
		exit(run_builtin(sh, cmd));
	exec_cmd(cmd, sh->envp);
	exit(127);
}

int	exec_pipeline_single(t_shell *sh, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (ms_cmd_is_empty(cmd))
		return (sh->exit_status = 0);
	if (cmd->argv && cmd->argv[0] && is_parent_builtin(cmd->argv[0]))
		return (run_parent_builtin(sh, cmd));
	signals_parent_exec();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), sh->exit_status = 1);
	if (pid == 0)
		child_exec(sh, cmd);
	waitpid(pid, &status, 0);
	sh->exit_status = ms_status_to_exit(status);
	signals_interactive();
	return (sh->exit_status);
}
