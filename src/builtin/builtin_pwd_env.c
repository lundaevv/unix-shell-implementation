#include "minishell.h"

int bi_pwd(void)
{
    char buf[4096];

    if (!getcwd(buf, sizeof(buf)))
    {
        perror("pwd");
        return 1;
    }
    write(1, buf, ft_strlen(buf));
    write(1, "\n", 1);
    return 0;
}

int bi_env(t_shell *sh)
{
    env_print(sh->envp);
    return 0;
}
