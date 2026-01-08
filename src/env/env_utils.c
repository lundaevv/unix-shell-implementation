#include "minishell.h"

static int env_count(char **envp)
{
    int i = 0;
    while (envp && envp[i])
        i++;
    return i;
}

static int env_find_index(char **envp, const char *name)
{
    size_t n;
    int i;

    if (!envp || !name)
        return -1;
    n = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, n) == 0 && envp[i][n] == '=')
            return i;
        i++;
    }
    return -1;
}

static char *make_kv(const char *name, const char *value)
{
    char *tmp;
    char *kv;

    tmp = ft_strjoin(name, "=");
    if (!tmp)
        return NULL;
    kv = ft_strjoin(tmp, value ? value : "");
    free(tmp);
    return kv;
}

int env_set(t_shell *sh, const char *name, const char *value)
{
    int idx;
    int count;
    char *kv;
    char **newenv;

    if (!sh || !sh->envp || !name || !*name)
        return 1;

    kv = make_kv(name, value);
    if (!kv)
        return 1;

    idx = env_find_index(sh->envp, name);
    if (idx >= 0)
    {
        free(sh->envp[idx]);
        sh->envp[idx] = kv;
        return 0;
    }

    count = env_count(sh->envp);
    newenv = malloc(sizeof(char *) * (count + 2));
    if (!newenv)
    {
        free(kv);
        return 1;
    }

    for (int i = 0; i < count; i++)
        newenv[i] = sh->envp[i];
    newenv[count] = kv;
    newenv[count + 1] = NULL;

    free(sh->envp);
    sh->envp = newenv;
    return 0;
}

int env_unset(t_shell *sh, const char *name)
{
    int idx;
    int count;

    if (!sh || !sh->envp || !name || !*name)
        return 0;

    idx = env_find_index(sh->envp, name);
    if (idx < 0)
        return 0;

    count = env_count(sh->envp);
    free(sh->envp[idx]);

    for (int i = idx; i < count; i++)
        sh->envp[i] = sh->envp[i + 1];
    return 0;
}

