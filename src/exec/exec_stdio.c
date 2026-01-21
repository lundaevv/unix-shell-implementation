#include "minishell.h"

int ms_stdio_save(int *in, int *out)
{
    *in = dup(STDIN_FILENO);
    *out = dup(STDOUT_FILENO);
    if (*in < 0 || *out < 0)
        return (perror("dup"), 1);
    return (0);
}

void ms_stdio_restore(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);
}
