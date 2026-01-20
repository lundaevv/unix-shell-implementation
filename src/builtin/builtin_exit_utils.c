#include "minishell.h"

static int ms_is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r');
}

static int ms_is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

/*
Checks if string is a valid integer format:
  optional spaces
  optional +/-
  digits
  optional spaces
*/
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


static int	parse_sign(const char *s, int *i)
{
	int	sign;

	sign = 1;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int	accumulate_digits(const char *s, int *i,
	unsigned long long *acc, int sign)
{
	while (ms_is_digit(s[*i]))
	{
		*acc = *acc * 10ULL + (s[*i] - '0');
		if (sign == 1 && *acc > (unsigned long long)LLONG_MAX)
			return (0);
		if (sign == -1 && *acc > (unsigned long long)LLONG_MAX + 1ULL)
			return (0);
		(*i)++;
	}
	return (1);
}
int	ms_atoll_checked(const char *s, long long *out)
{
	int					i;
	int					sign;
	unsigned long long	acc;

	if (!ms_is_strict_number(s) || !out)
		return (0);
	i = 0;
	while (ms_is_space(s[i]))
		i++;
	sign = parse_sign(s, &i);
	acc = 0;
	if (!accumulate_digits(s, &i, &acc, sign))
		return (0);
	if (sign == -1 && acc == (unsigned long long)LLONG_MAX + 1ULL)
		*out = LLONG_MIN;
	else
		*out = (long long)acc * sign;
	return (1);
}

