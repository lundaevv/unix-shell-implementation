/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:08:32 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 20:28:00 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_has_slash(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	*ms_join_path(const char *dir, const char *cmd)
{
	char	*res;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	res = malloc(len);
	if (!res)
		return (NULL);
	ft_strlcpy(res, dir, len);
	ft_strlcat(res, "/", len);
	ft_strlcat(res, cmd, len);
	return (res);
}

static char	*resolve_direct_path(const char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*resolve_path(const char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*full;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if (ms_has_slash(cmd))
		return (resolve_direct_path(cmd));
	path_env = get_env_value("PATH", envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths && paths[i])
	{
		full = ms_join_path(paths[i], cmd);
		i++;
		if (full && access(full, X_OK) == 0)
			return (free_split(paths), full);
		free(full);
	}
	return (free_split(paths), NULL);
}
