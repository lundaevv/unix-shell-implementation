/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:05:34 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/21 20:40:49 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_get_target(t_shell *sh, t_cmd *cmd, char **out)
{
	*out = NULL;
	if (cmd && cmd->argv)
		*out = cmd->argv[1];
	if (!*out)
		*out = env_get(sh->envp, "HOME");
	if (!*out)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	return (0);
}

static void	cd_update_pwd(t_shell *sh, const char *oldpwd)
{
	char	newpwd[4096];

	if (getcwd(newpwd, sizeof(newpwd)))
	{
		env_set(sh, "OLDPWD", oldpwd);
		env_set(sh, "PWD", newpwd);
	}
}

int	bi_cd(t_shell *sh, t_cmd *cmd)
{
	char	*path;
	char	oldpwd[4096];

	if (!sh)
		return (1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (cd_get_target(sh, cmd, &path) != 0)
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	cd_update_pwd(sh, oldpwd);
	return (0);
}
