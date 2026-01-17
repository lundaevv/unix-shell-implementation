#include "minishell.h"

static int  count_words(char **w)
{
    int i = 0;
    while (w && w[i])
        i++;
    return i;
}

int parse_stub(char *line, t_pipeline *p)
{
    char **argv;

    argv = ft_split(line, ' ');
    if (!argv || count_words(argv) == 0)
        return (free_split(argv), 1);
    p->cmds = malloc(sizeof(t_cmd));
    if (!p->cmds)
        return (free_split(argv), 1);
    p->count = 1;
    p->cmds[0].argv = argv;
    p->cmds[0].redirs = NULL;
    p->cmds[0].redir_count = 0;
    return (0);
}

void free_pipeline_stub(t_pipeline *p)
{
    if (!p || !p->cmds)
        return ;
    free_split(p->cmds[0].argv);
    free(p->cmds);
    p->cmds = NULL;
    p->count = 0;
}
