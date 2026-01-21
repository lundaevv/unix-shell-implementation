#include "minishell.h"

static int	open_infile(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror(file);
	return (fd);
}

static int	open_outfile(const char *file, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
		perror(file);
	return (fd);
}

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

static int	apply_one_redir(t_shell *sh, t_redir *r)
{
	int	fd;

	if (r->type == REDIR_IN)
	{
		fd = open_infile(r->target);
		if (fd < 0)
			return (1);
		return (dup2_and_close(fd, STDIN_FILENO));
	}
	if (r->type == REDIR_OUT)
	{
		fd = open_outfile(r->target, 0);
		if (fd < 0)
			return (1);
		return (dup2_and_close(fd, STDOUT_FILENO));
	}
	if (r->type == REDIR_APPEND)
	{
		fd = open_outfile(r->target, 1);
		if (fd < 0)
			return (1);
		return (dup2_and_close(fd, STDOUT_FILENO));
	}
	fd = heredoc_open(sh, r->target, r->heredoc_expand);
	if (fd == -130)
		return (130);
	if (fd < 0)
		return (1);
	return (dup2_and_close(fd, STDIN_FILENO));
}


int	apply_redirections(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	r;

	if (!cmd || cmd->redir_count <= 0 || !cmd->redirs)
		return (0);
	i = 0;
	while (i < cmd->redir_count)
	{
		r = apply_one_redir(sh, &cmd->redirs[i]);
		if (r != 0)
			return (r);
		i++;
	}
	return (0);
}
