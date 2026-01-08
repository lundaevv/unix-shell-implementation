#include "minishell.h"

int bi_exit(t_shell *sh, t_cmd *cmd)
{
    (void)cmd;
    sh->should_exit = 1;
    return sh->exit_status;
}
