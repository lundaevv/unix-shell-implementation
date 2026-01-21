#include "minishell.h"

void ms_close_all_pipes(int pipes[][2], int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

static int make_pipes(int pipes[][2], int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        if (pipe(pipes[i]) < 0)
            return (perror("pipe"), 1);
        i++;
    }
    return (0);
}

int exec_pipeline_multi(t_shell *sh, t_pipeline *p)
{
    int pipe_count;
    int pipes[1024][2];
    pid_t pids[1024];
    int i;

    pipe_count = p->cmd_count - 1;
    if (pipe_count > 1024 || make_pipes(pipes, pipe_count))
        return (sh->exit_status = 1);
    signals_parent_exec();
    i = 0;
    while (i < p->cmd_count)
    {
        pids[i] = fork();
        if (pids[i] < 0)
            return (perror("fork"), ms_close_all_pipes(pipes, pipe_count), 1);
        if (pids[i] == 0)
            child_run_pipeline_cmd(sh, p, i, pipes);
        i++;
    }
    ms_close_all_pipes(pipes, pipe_count);
    sh->exit_status = ms_wait_last(pids, p->cmd_count);
    signals_interactive();
    return (sh->exit_status);
}
