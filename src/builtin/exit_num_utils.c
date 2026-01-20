#include "minishell.h"

int ms_is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r');
}

int ms_is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

int ms_is_strict_number(const char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (ms_is_space(s[i]))
        i++;
    if (s[i] == '+' || s[i] == '-')
        i++;
    if (!ms_is_digit(s[i]))
        return (0);
    while (ms_is_digit(s[i]))
        i++;
    while (ms_is_space(s[i]))
        i++;
    return (s[i] == '\0');
}

