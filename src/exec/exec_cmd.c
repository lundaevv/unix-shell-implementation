#include "minishell.h"

void exec_cmd(t_cmd *cmd, char **envp)
{
    char *path;

    if (!cmd->argv || !cmd->argv[0])
        exit(0);

    path = resolve_path(cmd->argv[0], envp);
    if (!path)
    {
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }

    execve(path, cmd->argv, envp);
    perror(cmd->argv[0]);
    free(path);
    exit(127);
}

