/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:07:04 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 19:25:38 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*hd_read_line(void)
{
	char	*line;
	char	c;
	ssize_t	r;

	line = NULL;
	while (1)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r < 0)
		{
			if (errno == EINTR)
				g_signal = SIGINT;
			break ;
		}
		if (r == 0)
			break ;
		line = hd_str_add_char(line, c);
		if (!line)
			return (NULL);
		if (c == '\n')
			break ;
	}
	return (line);
}

static int	hd_is_limiter(char *line, const char *limiter)
{
	size_t	n;

	if (!line || !limiter)
		return (0);
	n = ft_strlen(limiter);
	return (ft_strncmp(line, limiter, n) == 0 && line[n] == '\n');
}

static int	hd_expand_and_write(int wfd, t_shell *sh, char *line)
{
	char	*out;

	out = ms_expand_unquote(line, sh->envp, sh->exit_status);
	free(line);
	if (!out)
		return (1);
	hd_write_line(wfd, out);
	return (0);
}

static int	hd_fill(int wfd, t_shell *sh, const char *lim, int expand)
{
	char	*line;

	while (1)
	{
		if (g_signal == SIGINT)
			return (130);
		if (isatty(STDIN_FILENO))
			write (STDOUT_FILENO, "> ", 2);
		line = hd_read_line();
		if (!line)
		{
			if (g_signal == SIGINT)
				return (130);
			break ;
		}
		if (g_signal == SIGINT)
			return (free(line), 130);
		if (hd_is_limiter(line, lim))
			return (free(line), 0);
		if (!expand)
			hd_write_line(wfd, line);
		else if (hd_expand_and_write(wfd, sh, line) != 0)
			return (1);
	}
	return (0);
}

int	heredoc_open(t_shell *sh, const char *limiter, int expand)
{
	int	hd[2];
	int	ret;

	if (pipe(hd) < 0)
		return (perror("pipe"), -1);
	signals_heredoc();
	g_signal = 0;
	ret = hd_fill(hd[1], sh, limiter, expand);
	close(hd[1]);
	if (ret == 130)
		return (close(hd[0]), -130);
	if (ret != 0)
		return (close(hd[0]), -1);
	return (hd[0]);
}
