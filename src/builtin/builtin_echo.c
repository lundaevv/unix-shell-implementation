#include "minishell.h"

int bi_echo(t_cmd *cmd)
{
    int i = 1;
    int newline = 1;

    if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-n", 3) == 0)
    {
        newline = 0;
        i = 2;
    }
    while (cmd->argv[i])
    {
        write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
        if (cmd->argv[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (newline)
        write(1, "\n", 1);
    return 0;
}
