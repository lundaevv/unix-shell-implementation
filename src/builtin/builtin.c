/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:05:52 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 15:52:25 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Small helper: strict string equality.
** Returns 1 if strings are identical, else 0.
*/
static int	str_eq(const char *a, const char *b)
{
	size_t	la;
	size_t	lb;

	if (!a || !b)
		return (0);
	la = ft_strlen(a);
	lb = ft_strlen(b);
	if (la != lb)
		return (0);
	return (ft_strncmp(a, b, la) == 0);
}

/*
** True for any builtin we support.
** Used in children and pipelines (builtins can run in child too).
*/
int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	return (str_eq(name, "echo") || str_eq(name, "cd") || str_eq(name, "pwd")
		|| str_eq(name, "export") || str_eq(name, "unset") || str_eq(name,
			"env") || str_eq(name, "exit"));
}

/*
** Builtins that MUST run in the parent process because they change shell state:
** - cd changes cwd
** - export/unset changes env
** - exit changes should_exit
*/
int	is_parent_builtin(const char *name)
{
	if (!name)
		return (0);
	return (str_eq(name, "cd") || str_eq(name, "export") || str_eq(name,
			"unset") || str_eq(name, "exit"));
}

/*
** Dispatch builtin execution.
** Returns the builtin's exit status.
*/
int	run_builtin(t_shell *sh, t_cmd *cmd)
{
	const char	*name;

	if (ms_cmd_is_empty(cmd))
		return (sh->exit_status = 0);
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	name = cmd->argv[0];
	if (str_eq(name, "echo"))
		return (bi_echo(cmd));
	if (str_eq(name, "pwd"))
		return (bi_pwd());
	if (str_eq(name, "env"))
		return (bi_env(sh));
	if (str_eq(name, "cd"))
		return (bi_cd(sh, cmd));
	if (str_eq(name, "export"))
		return (bi_export(sh, cmd));
	if (str_eq(name, "unset"))
		return (bi_unset(sh, cmd));
	if (str_eq(name, "exit"))
		return (bi_exit(sh, cmd));
	return (1);
}
