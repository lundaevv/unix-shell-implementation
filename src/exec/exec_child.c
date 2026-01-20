#include "minishell.h"

/*
This file contains the function that runs inside EACH child process of a pipeline.

It is called only from exec_pipeline_multi() after fork():

    if (pids[i] == 0)
        child_run_pipeline_cmd(sh, p, i, pipes);

So: this code never runs in the parent.
It always runs in the forked child process.

Goal of this function:
1) Connect pipes to stdin/stdout for this command (dup2)
2) Close ALL pipe fds (child must not keep extra fds open)
3) Apply redirections (< > >> <<)
   IMPORTANT: redirections override pipes if both exist
4) Run builtins or external command
5) If something fails -> exit with correct status

PIPELINE EXAMPLE:
    cmd0 | cmd1 | cmd2

- cmd0:
    stdin  = normal
    stdout = pipes[0][1]
- cmd1:
    stdin  = pipes[0][0]
    stdout = pipes[1][1]
- cmd2:
    stdin  = pipes[1][0]
    stdout = normal
 */

void child_run_pipeline_cmd(t_shell *sh, t_pipeline *p, int i, int pipes[][2])
{
    int     pipe_count;
    int     k;
    t_cmd   *cmd;

    /* Child process must have default Ctrl-C / Ctrl-\ behavior
       (parent ignores signals, child must be killable like bash). */
    signals_child_exec();

    cmd = &p->cmds[i];
    pipe_count = p->count - 1;

    /*
    1) CONNECT STDIN FROM PREVIOUS PIPE (if not the first command)

    If i > 0, this command should read from the previous pipe's READ end.
    dup2 replaces STDIN_FILENO with that pipe fd.
    */
    if (i > 0)
    {
        if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }
    }

    /*
    2) CONNECT STDOUT TO NEXT PIPE (if not the last command)

    If i < last, this command should write into the next pipe's WRITE end.
    dup2 replaces STDOUT_FILENO with that pipe fd.
    */
    if (i < p->count - 1)
    {
        if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }
    }

    /*
    3) CLOSE ALL PIPE FDS IN CHILD

    After dup2, we must close ALL pipe fd copies.
    If we don't close them:
      - pipes never get EOF
      - commands like `cat | wc` may hang forever
    */
    k = 0;
    while (k < pipe_count)
    {
        close(pipes[k][0]);
        close(pipes[k][1]);
        k++;
    }

    /*
    4) APPLY REDIRECTIONS

    This is done AFTER pipe dup2 on purpose.
    Why? Because redirections must override pipes:

        echo hi | cat > out.txt

    For cmd "cat": stdout first becomes pipe, then > out.txt replaces it.
    So output goes to file, not to the pipe.

    apply_redirections() returns:
      0   -> ok
      1   -> general error
      130 -> heredoc was canceled by Ctrl-C
    We exit with that status so parent gets correct exit_status.
    */
    {
        int r = apply_redirections(sh, cmd);
        if (r != 0)
            exit(r);
    }

    /*
    5) EXECUTE COMMAND

    In a pipeline, builtins must run inside child because they should NOT
    affect the parent shell state. Example:

        export X=1 | cat

    If export ran in parent, X would remain set, but bash does NOT do that.
    So in pipeline: builtins run like normal child commands (exit with status).
    */
    if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
        exit(run_builtin(sh, cmd));

    /*
    External command:
    exec_cmd does PATH resolution and execve().
    If execve fails, exec_cmd must exit with correct code (like 127).
    */
    exec_cmd(cmd, sh->envp);
}



