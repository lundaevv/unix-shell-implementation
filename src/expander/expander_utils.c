#include "minishell.h"

int	ms_is_var_start(char c)
{
	return (ft_isalpha((unsigned char)c) || c == '_');
}

int	ms_is_var_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}

int	ms_var_name_len(const char *s)
{
	int	len;

	len = 0;
	while (s[len] && ms_is_var_char(s[len]))
		len++;
	return (len);
}

char	*ms_get_env_value(const char *name, int len, char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
