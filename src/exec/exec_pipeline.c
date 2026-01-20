#include "minishell.h"

int exec_pipeline(t_shell *sh, t_pipeline *p)
{
    if (!p || p->count <= 0)
        return (sh->exit_status = 0);
    if (p->count == 1)
        return (exec_pipeline_single(sh, &p->cmds[0]));
    return (exec_pipeline_multi(sh, p));
}

