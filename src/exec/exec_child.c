#include "minishell.h"

void child_run_pipeline_cmd(t_shell *sh, t_pipeline *p, int i, int pipes[][2])
{
    int pipe_count;
    int k;
    t_cmd *cmd;

    cmd = &p->cmds[i];
    pipe_count = p->count - 1;

    if (i > 0)
    {
        if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }
    }

    if (i < p->count - 1)
    {
        if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }
    }

    k = 0;
    while (k < pipe_count)
    {
        close(pipes[k][0]);
        close(pipes[k][1]);
        k++;
    }

    if (apply_redirections(cmd) != 0)
        exit(1);

    if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
        exit(run_builtin(sh, cmd));
    exec_cmd(cmd, sh->envp);
}



