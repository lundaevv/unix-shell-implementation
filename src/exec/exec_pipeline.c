
#include "minishell.h"

static void close_all_pipes(int pipes[][2], int n)
{
    int i = 0;
    while (i < n)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

static void wait_all(pid_t *pids, int count, int *exit_status)
{
    int i;
    int status;

    i = 0;
    while (i < count)
    {
        waitpid(pids[i], &status, 0);
        if (i == count - 1)
        {
            if (WIFEXITED(status))
                *exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                *exit_status = 128 + WTERMSIG(status);
            else
                *exit_status = 1;
        }
        i++;
    }
}

int exec_pipeline_multi(t_shell *sh, t_pipeline *p)
{
    int     i;
    int     pipe_count;
    int     pipes[1024][2];
    pid_t   pids[1024];

    if (p->count <= 1)
        return 0;

    pipe_count = p->count - 1;
    if (pipe_count > 1024)
        return 1;

    i = 0;
    while (i < pipe_count)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            sh->exit_status = 1;
            return 1;
        }
        i++;
    }
    i = 0;
    while (i < p->count)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("fork");
            close_all_pipes(pipes, pipe_count);
            sh->exit_status = 1;
            return 1;
        }
        if (pids[i] == 0)
            child_run_pipeline_cmd(sh, p, i, pipes);
        i++;
    }
    close_all_pipes(pipes, pipe_count);
    wait_all(pids, p->count, &sh->exit_status);
    return sh->exit_status;
}



static void restore_stdio(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);
}

int exec_pipeline(t_shell *sh, t_pipeline *p)
{
    t_cmd *cmd;

    if (p->count == 1)
    {
        cmd = &p->cmds[0];

        if (cmd->argv && cmd->argv[0] && is_parent_builtin(cmd->argv[0]))
        {
            int saved_in = dup(STDIN_FILENO);
            int saved_out = dup(STDOUT_FILENO);

            if (apply_redirections(cmd) != 0)
                sh->exit_status = 1;
            else
                sh->exit_status = run_builtin(sh, cmd);

            restore_stdio(saved_in, saved_out);
            return sh->exit_status;
        }
        pid_t pid = fork();
        int status;

        if (pid < 0) { perror("fork"); sh->exit_status = 1; return 1; }
        if (pid == 0)
        {
            if (apply_redirections(cmd) != 0)
                exit(1);
            if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
                exit(run_builtin(sh, cmd));
            exec_cmd(cmd, sh->envp);
        }
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) sh->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status)) sh->exit_status = 128 + WTERMSIG(status);
        else sh->exit_status = 1;
        return sh->exit_status;
    }

    return exec_pipeline_multi(sh, p);
}
