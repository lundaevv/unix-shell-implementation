/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:08:28 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 17:25:42 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_status_to_exit(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	ms_wait_last(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	last;

	last = 0;
	i = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &status, 0) < 0)
			last = 1;
		if (i == n - 1)
			last = ms_status_to_exit(status);
		i++;
	}
	return (last);
}
