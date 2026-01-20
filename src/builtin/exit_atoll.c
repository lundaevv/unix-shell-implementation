#include "minishell.h"

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

