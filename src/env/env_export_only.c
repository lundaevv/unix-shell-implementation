/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export_only.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 13:10:30 by gperedny          #+#    #+#             */
/*   Updated: 2026/01/23 13:17:02 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_find_index_any(char **envp, const char *name)
{
	size_t	n;
	int		i;

	if (!envp || !name || !*name)
		return (-1);
	n = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, n) == 0
			&& (envp[i][n] == '=' || envp[i][n] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	env_export_only(t_shell *sh, const char *name)
{
	int		idx;
	char	*dup;

	if (!sh || !sh->envp || !name || !*name)
		return (1);
	idx = env_find_index_any(sh->envp, name);
	if (idx >= 0)
		return (0);
	dup = ft_strdup(name);
	if (!dup)
		return (1);
	if (env_append_kv(sh, dup) != 0)
		return (free(dup), 1);
	return (0);
}
