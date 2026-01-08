#include "minishell.h"

/* minimal: only supports export KEY=VALUE, no printing sorting yet */
int bi_export(t_shell *sh, t_cmd *cmd)
{
    char *arg;
    char *eq;

    if (!cmd->argv[1])
    {
        /* optional: implement export_print later */
        return 0;
    }

    arg = cmd->argv[1];
    eq = ft_strchr(arg, '=');
    if (!eq)
    {
        return env_set(sh, arg, "");
    }
    *eq = '\0';
    int ret = env_set(sh, arg, eq + 1);
    *eq = '=';
    return ret;
}

int bi_unset(t_shell *sh, t_cmd *cmd)
{
    if (!cmd->argv[1])
        return 0;
    return env_unset(sh, cmd->argv[1]);
}
