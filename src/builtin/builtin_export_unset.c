/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_unset.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:05:46 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/23 13:12:26 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_one(t_shell *sh, char *arg)
{
	char	*eq;
	char	*name;
	int		ret;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (env_export_only(sh, arg));
	name = ft_substr(arg, 0, (size_t)(eq - arg));
	if (!name)
		return (1);
	ret = env_set(sh, name, eq + 1);
	free(name);
	return (ret);
}

static int	export_name_ok(const char *arg)
{
	int		len;
	char	*eq;
	char	*name;
	int		ok;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (ms_is_valid_ident(arg));
	len = (int)(eq - arg);
	if (len <= 0)
		return (0);
	name = ft_substr(arg, 0, (size_t)len);
	if (!name)
		return (0);
	ok = ms_is_valid_ident(name);
	free(name);
	return (ok);
}
/*
static int	ms_export_name_ok(char *arg)
{
	char	*eq;
	int		ok;
	char	save;

	if (!arg || !*arg)
		return (0);
	eq = ft_strchr(arg, '=');
	if (!eq)
		return (ms_is_valid_ident(arg));
	save = *eq;
	*eq = '\0';
	ok = ms_is_valid_ident(arg);
	*eq = save;
	return (ok);
}
*/

int	bi_export(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->argv)
		return (1);
	if (!cmd->argv[1])
		return (export_print(sh->envp), 0);
	i = 1;
	ret = 0;
	while (cmd->argv[i])
	{
		if (!export_name_ok(cmd->argv[i]))
		{
			ms_err_ident("export", cmd->argv[i]);
			ret = 1;
		}
		else if (export_one(sh, cmd->argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}

int	bi_unset(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->argv)
		return (1);
	if (!cmd->argv[1])
		return (0);
	i = 1;
	ret = 0;
	while (cmd->argv[i])
	{
		if (!ms_is_valid_ident(cmd->argv[i]))
		{
			ms_err_ident("unset", cmd->argv[i]);
			ret = 1;
		}
		else if (env_unset(sh, cmd->argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
