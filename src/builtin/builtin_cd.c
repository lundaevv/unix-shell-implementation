#include "minishell.h"

int bi_cd(t_shell *sh, t_cmd *cmd)
{
    char *path = cmd->argv[1];
    char oldpwd[4096];
    char newpwd[4096];

    if (!getcwd(oldpwd, sizeof(oldpwd)))
        oldpwd[0] = '\0';

    if (!path)
        path = env_get(sh->envp, "HOME");

    if (!path)
    {
        write(2, "cd: HOME not set\n", 17);
        return 1;
    }

    if (chdir(path) != 0)
    {
        perror("cd");
        return 1;
    }

    if (getcwd(newpwd, sizeof(newpwd)))
    {
        env_set(sh, "OLDPWD", oldpwd);
        env_set(sh, "PWD", newpwd);
    }
    return 0;
}
