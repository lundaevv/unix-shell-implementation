#include "minishell.h"

/* minimal: only supports export KEY=VALUE, no printing sorting yet */
int bi_export(t_shell *sh, t_cmd *cmd)
{
    char    *arg;
    char    *eq;
    char    *name;
    int     ret;

    if (!cmd->argv[1])
        return 0;
    arg = cmd->argv[1];
    eq = ft_strchr(arg, '=');
    if (!eq)
        return env_set(sh, arg, "");
    name = ft_substr(arg, 0, (size_t)(eq - arg));
    if (!name)
        return 1;
    ret = env_set(sh, name, eq + 1);
    free(name);
    return ret;
}

int bi_unset(t_shell *sh, t_cmd *cmd)
{
    if (!cmd->argv[1])
        return 0;
    return env_unset(sh, cmd->argv[1]);
}
