#include "minishell.h"

static int open_infile(const char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror(file);
	return (fd);
}
static int open_outfile(const char *file, int append)
{
    int flags = O_WRONLY | O_CREAT;

    if (append)
        flags |= O_APPEND;
    else
        flags |= O_TRUNC;

    int fd = open(file, flags, 0644);
    if (fd < 0)
        perror(file);
    return fd;
}

int apply_redirections(t_cmd *cmd)
{
	int i;
	int fd;

	if(!cmd || cmd->redir_count <= 0 || !cmd->redirs)
		return (0);
	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_IN)
		{
			fd = open_infile(cmd->redirs[i].filename);
			if (fd < 0)
				return (1);
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_OUT)
		{
			fd = open_outfile(cmd->redirs[i].filename, 0);
			if (fd < 0)
				return (1);
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_APPEND)
		{
			fd = open_outfile(cmd->redirs[i].filename, 1);
			if (fd < 0)
				return (1);
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_HEREDOC)
		{
			// Heredoc redirection
			// We skip its implementation
		}
		i++;
	}
	return (0);
}
