#include "minishell.h"

char **env_copy(char **envp)
{
    int i = 0;
    while (envp && envp[i])
        i++;

    char **copy = malloc(sizeof(char *) * (i + 1));
    if (!copy)
        return NULL;

    i = 0;
    while (envp && envp[i])
    {
        copy[i] = ft_strdup(envp[i]);
        if (!copy[i])
        {
            while (i > 0)
                free(copy[--i]);
            free(copy);
            return NULL;
        }
        i++;
    }
    copy[i] = NULL;
    return copy;
}

char *env_get(char **envp, const char *name)
{
    size_t n = ft_strlen(name);
    int i = 0;

    while (envp && envp[i])
    {
        if (ft_strncmp(envp[i], name, n) == 0 && envp[i][n] == '=')
            return envp[i] + n + 1;
        i++;
    }
    return NULL;
}

void env_print(char **envp)
{
    int i = 0;
    while (envp && envp[i])
    {
        /* env prints only KEY=VALUE lines */
        if (ft_strchr(envp[i], '='))
        {
            write(STDOUT_FILENO, envp[i], ft_strlen(envp[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
        i++;
    }
}
