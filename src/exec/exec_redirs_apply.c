/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs_apply.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:29:55 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 19:45:52 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup2_and_close(int fd, int target_fd)
{
	if (dup2(fd, target_fd) < 0)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	apply_file_in(t_redir *r)
{
	int	fd;

	fd = open(r->target, O_RDONLY);
	if (fd < 0)
		return (perror(r->target), 1);
	return (dup2_and_close(fd, STDIN_FILENO));
}

static int	apply_file_out(t_redir *r, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(r->target, flags, 0644);
	if (fd < 0)
		return (perror(r->target), 1);
	return (dup2_and_close(fd, STDOUT_FILENO));
}

static int	apply_heredoc(t_shell *sh, t_redir *r)
{
	(void)sh;
	if (r->hd_fd < 0)
		return (1);
	return (dup2_and_close(r->hd_fd, STDIN_FILENO));
}

int	apply_one_redir(t_shell *sh, t_redir *r)
{
	if (r->type == REDIR_IN)
		return (apply_file_in(r));
	if (r->type == REDIR_OUT)
		return (apply_file_out(r, 0));
	if (r->type == REDIR_APPEND)
		return (apply_file_out(r, 1));
	return (apply_heredoc(sh, r));
}
