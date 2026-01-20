#include "minishell.h"

int ms_status_to_exit(int status)
{
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}

int ms_wait_last(pid_t *pids, int n)
{
    int i;
    int status;
    int last;

    last = 1;
    i = 0;
    while (i < n)
    {
        waitpid(pids[i], &status, 0);
        if (i == n - 1)
            last = ms_status_to_exit(status);
        i++;
    }
    return (last);
}
