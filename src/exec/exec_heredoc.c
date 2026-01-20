#include "minishell.h"

static char	*hd_str_add_char(char *s, char c)
{
	char	*new;
	size_t	len;

	len = 0;
	if (s)
		len = ft_strlen(s);
	new = malloc(len + 2);
	if (!new)
		return (free(s), NULL);
	if (s)
		ft_memcpy(new, s, len);
	new[len] = c;
	new[len + 1] = '\0';
	free(s);
	return (new);
}

static char	*hd_read_line(void)
{
	char	*line;
	char	c;
	ssize_t	r;

	line = NULL;
	while (1)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r <= 0)
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

static int	hd_write_line(int wfd, char *line)
{
	write(wfd, line, ft_strlen(line));
	free(line);
	return (0);
}

static int	hd_fill(int wfd, t_shell *sh, const char *lim, int expand)
{
	char	*line;
	char	*out;

	while (1)
	{
		if (g_signal == SIGINT)
			return (130);
		write(STDOUT_FILENO, "> ", 2);
		line = hd_read_line();
		if (!line)
			break ;
		if (g_signal == SIGINT)
			return (free(line), 130);
		if (hd_is_limiter(line, lim))
			return (free(line), 0);
		if (!expand)
			hd_write_line(wfd, line);
		else
		{
			out = ms_expand_unquote(line, sh->envp, sh->exit_status);
			free(line);
			if (!out)
				return (1);
			hd_write_line(wfd, out);
		}
	}
	return (0);
}

int	heredoc_open(t_shell *sh, const char *limiter, int expand)
{
	int	hd[2];
	int	ret;

	if (pipe(hd) < 0)
		return (perror("pipe"), -1);
	g_signal = 0;
	ret = hd_fill(hd[1], sh, limiter, expand);
	close(hd[1]);
	if (ret == 130)
		return (close(hd[0]), -130);
	if (ret != 0)
		return (close(hd[0]), -1);
	return (hd[0]);
}
