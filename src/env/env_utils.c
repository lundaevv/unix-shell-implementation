/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:09:02 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/23 13:13:29 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

//static int	env_find_index(char **envp, const char *name)
//{
//	size_t	n;
//	int		i;

//	if (!envp || !name)
//		return (-1);
//	n = ft_strlen(name);
//	i = 0;
//	while (envp[i])
//	{
//		if (ft_strncmp(envp[i], name, n) == 0 && envp[i][n] == '=')
//			return (i);
//		i++;
//	}
//	return (-1);
//}

static char	*make_kv(const char *name, const char *value)
{
	char	*tmp;
	char	*kv;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	if (value)
		kv = ft_strjoin(tmp, value);
	else
		kv = ft_strjoin(tmp, "");
	free(tmp);
	return (kv);
}

int	env_unset(t_shell *sh, const char *name)
{
	int	idx;
	int	count;

	if (!sh || !sh->envp || !name || !*name)
		return (0);
	idx = env_find_index_any(sh->envp, name);
	if (idx < 0)
		return (0);
	count = env_count(sh->envp);
	free(sh->envp[idx]);
	while (idx < count)
	{
		sh->envp[idx] = sh->envp[idx + 1];
		idx++;
	}
	return (0);
}

int	env_set(t_shell *sh, const char *name, const char *value)
{
	int		idx;
	char	*kv;

	if (!sh || !sh->envp || !name || !*name)
		return (1);
	kv = make_kv(name, value);
	if (!kv)
		return (1);
	idx = env_find_index_any(sh->envp, name);
	if (idx >= 0)
	{
		free(sh->envp[idx]);
		sh->envp[idx] = kv;
		return (0);
	}
	if (env_append_kv(sh, kv) != 0)
		return (free(kv), 1);
	return (0);
}
