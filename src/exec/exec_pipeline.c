
#include "minishell.h"

/*
I have to edit this one for the NORMS.
This file is the "brain" of execution.
It receives a ready t_pipeline from parser and runs it.

The parser gives me:
    p->count          = number of commands
    p->cmds[i].argv   = argv array (NULL-terminated)
    p->cmds[i].redirs = redirections array
    p->cmds[i].redir_count

I only execute.

This file implements:
    - exec_pipeline() : entry point for execution
    - exec_pipeline_multi() : pipeline execution (cmd1 | cmd2 | cmd3 ...)
and some small helpers.

MAIN RULES:
1) If pipeline has ONE command:
      - If it's a "parent builtin" (cd/export/unset/exit) -> run WITHOUT fork
      - Otherwise -> fork and run in child
2) If pipeline has MULTIPLE commands:
      - create N-1 pipes
      - fork N children
      - connect pipes with dup2 in child_run_pipeline_cmd()
      - wait all children, set exit_status from the last command

SIGNALS RULES:
- Before forking: parent ignores SIGINT/SIGQUIT (signals_parent_exec)
- Child restores default signals (signals_child_exec)
- After execution: return to interactive signals (signals_interactive)

EXIT STATUS RULE:
- For pipelines: the exit status is the LAST command in the pipeline
- If child exited normally: use exit code
- If child died by signal: status = 128 + signal_number
  (bash style: Ctrl-C => 130, Ctrl-\ => 131)
 */

/* Close all pipe fd's (used in parent when we are done forking). */
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

/*
Wait all children and set exit_status from the LAST process.
We do not care about intermediate statuses except the last one.
*/
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

/*
Execute pipeline with multiple commands:
    cmd0 | cmd1 | cmd2 | ... | cmdN

Steps:
1) Parent sets signal mode for execution
2) Create all pipes (N-1)
3) Fork N children
4) Each child calls child_run_pipeline_cmd() (does dup2 + redirs + exec/builtin)
5) Parent closes all pipe fds
6) Parent waits all children
7) Restore interactive signals

*/
int exec_pipeline_multi(t_shell *sh, t_pipeline *p)
{
    int     i;
    int     pipe_count;
    int     pipes[1024][2];
    pid_t   pids[1024];

    if (p->count <= 1)
        return (0);

    /* During execution parent ignores Ctrl-C and Ctrl-\ */
    signals_parent_exec();

    pipe_count = p->count - 1;
    if (pipe_count > 1024)
        return (1);

    /* Create all pipes upfront */
    i = 0;
    while (i < pipe_count)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            sh->exit_status = 1;
            return (1);
        }
        i++;
    }

    /* Fork one child per command */
    i = 0;
    while (i < p->count)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("fork");
            close_all_pipes(pipes, pipe_count);
            sh->exit_status = 1;
            return (1);
        }
        if (pids[i] == 0)
            child_run_pipeline_cmd(sh, p, i, pipes);
        i++;
    }

    /* Parent: close all pipes, then wait */
    close_all_pipes(pipes, pipe_count);
    wait_all(pids, p->count, &sh->exit_status);

    /* Return to interactive mode */
    signals_interactive();
    return (sh->exit_status);
}

/*
When we run a parent builtin (cd/export/unset/exit) we must not destroy stdin/stdout,
so we save them before applying redirections, and restore after builtin finishes.
*/
static void restore_stdio(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);
}

/*
Entry point for executor.

If p->count == 1:
    - if parent builtin: run in parent (no fork)
    - else fork child and run builtin/external inside child

If p->count > 1:
    - run as pipeline (exec_pipeline_multi)
*/
int exec_pipeline(t_shell *sh, t_pipeline *p)
{
    t_cmd *cmd;

    if (p->count == 1)
    {
        cmd = &p->cmds[0];

        /*
        Parent builtins must run in the parent process because they change shell state:
            cd      changes cwd
            export  changes env
            unset   changes env
            exit    sets should_exit
        We still support redirections for them, so we temporarily redirect stdin/stdout.
        */
        if (cmd->argv && cmd->argv[0] && is_parent_builtin(cmd->argv[0]))
        {
            int saved_in = dup(STDIN_FILENO);
            int saved_out = dup(STDOUT_FILENO);

            int r = apply_redirections(cmd);
            if (r != 0)
                sh->exit_status = r;
            else
                sh->exit_status = run_builtin(sh, cmd);

            restore_stdio(saved_in, saved_out);
            return (sh->exit_status);
        }

        /*
        Non-parent builtins (echo/pwd/env) can be executed in child like external commands.
        Also external commands must be forked.
        */
        signals_parent_exec();
        pid_t pid = fork();
        int status;

        if (pid < 0)
        {
            perror("fork");
            sh->exit_status = 1;
            return (1);
        }
        if (pid == 0)
        {
            /* Child must have default behavior for Ctrl-C / Ctrl-\ */
            signals_child_exec();

            /* Apply redirections inside child. If heredoc was Ctrl-C canceled -> exit 130 */
            int r = apply_redirections(cmd);
            if (r != 0)
                exit(r);

            /* Builtins in child */
            if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
                exit(run_builtin(sh, cmd));

            /* External command */
            exec_cmd(cmd, sh->envp);
        }

        /* Parent waits and sets exit status */
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            sh->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            sh->exit_status = 128 + WTERMSIG(status);

            /* Make prompt look like bash after signals */
            if (WTERMSIG(status) == SIGINT)
                write(1, "\n", 1);
            else if (WTERMSIG(status) == SIGQUIT)
                write(1, "Quit (core dumped)\n", 19);
        }
        else
            sh->exit_status = 1;

        /* Restore interactive signal behavior before going back to readline */
        signals_interactive();
        return (sh->exit_status);
    }

    /* Multiple commands: pipeline execution */
    return (exec_pipeline_multi(sh, p));
}

