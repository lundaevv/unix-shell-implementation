#include "minishell.h"

static int run_parent_builtin(t_shell *sh, t_cmd *cmd)
{
    int in;
    int out;
    int r;

    /* Bash prints "exit" only when exit is executed in the main shell */
    if (cmd->argv && cmd->argv[0]
        && ft_strncmp(cmd->argv[0], "exit", 5) == 0)
        write(1, "exit\n", 5);

    if (ms_stdio_save(&in, &out))
        return (sh->exit_status = 1);
    r = apply_redirections(sh, cmd);
    if (r == 0)
        r = run_builtin(sh, cmd);
    ms_stdio_restore(in, out);
    return (sh->exit_status = r);
}


static int child_exec(t_shell *sh, t_cmd *cmd)
{
    int r;

    signals_child_exec();
    r = apply_redirections(sh, cmd);
    if (r != 0)
        exit(r);
    if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
        exit(run_builtin(sh, cmd));
    exec_cmd(cmd, sh->envp);
    exit(127);
}

int exec_pipeline_single(t_shell *sh, t_cmd *cmd)
{
    pid_t pid;
    int status;

    if (cmd->argv && cmd->argv[0] && is_parent_builtin(cmd->argv[0]))
        return (run_parent_builtin(sh, cmd));
    signals_parent_exec();
    pid = fork();
    if (pid < 0)
        return (perror("fork"), sh->exit_status = 1);
    if (pid == 0)
        child_exec(sh, cmd);
    waitpid(pid, &status, 0);
    sh->exit_status = ms_status_to_exit(status);
    signals_interactive();
    return (sh->exit_status);
}
