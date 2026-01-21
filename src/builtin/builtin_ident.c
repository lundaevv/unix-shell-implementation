#include "minishell.h"

static int	ms_is_ident_start(char c)
{
	return (ft_isalpha((unsigned char)c) || c == '_');
}

static int	ms_is_ident_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}

int	ms_is_valid_ident(const char *s)
{
	int	i;

	if (!s || !s[0] || !ms_is_ident_start(s[0]))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ms_is_ident_char(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ms_err_ident(const char *cmd, const char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}
