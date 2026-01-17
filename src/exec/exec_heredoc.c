#include "minishell.h"


static char *str_add_char(char *s, char c)
{
    char    *new;
    size_t  len;

    len = 0;
    if (s)
        len = ft_strlen(s);
    new = malloc(len + 2);
    if (!new)
        return (free(s), NULL);
    if (s)
        ft_memcpy(new, s, len);
    new[len] = c;
    new[len + 1] = '\0';
    free(s);
    return (new);
}

static char *read_heredoc_line(void)
{
    char    *line;
    char    c;
    ssize_t r;

    line = NULL;
    while (1)
    {
        r = read(STDIN_FILENO, &c, 1);
        if (r <= 0)
            break;
        line = str_add_char(line, c);
        if (!line)
            return (NULL);
        if (c == '\n')
            break;
    }
    return (line);
}

static int is_limiter_line(char *line, const char *limiter)
{
    size_t n;

    if (!line || !limiter)
        return (0);
    n = ft_strlen(limiter);
    return (ft_strncmp(line, limiter, n) == 0 && line[n] == '\n');
}

static int heredoc_fill(int wfd, const char *limiter)
{
    char *line;

    while (1)
    {
        if (g_signal == SIGINT)
            return (130);
        write(STDOUT_FILENO, "> ", 2);
        line = read_heredoc_line();
        if (!line)
            break;
        if (g_signal == SIGINT)
            return (free(line), 130);
        if (is_limiter_line(line, limiter))
        {
            free(line);
            break;
        }
        write(wfd, line, ft_strlen(line));
        free(line);
    }
    return (0);
}

int heredoc_open(const char *limiter)
{
    int hd[2];
    int ret;

    if (pipe(hd) < 0)
    {
        perror("pipe");
        return (-1);
    }
    g_signal = 0;
    ret = heredoc_fill(hd[1], limiter);
    close(hd[1]);
    if (ret == 130)
    {
        close(hd[0]);
        return (-130);
    }
    return (hd[0]);
}
