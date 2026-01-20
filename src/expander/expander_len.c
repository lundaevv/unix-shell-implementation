#include "minishell.h"

static size_t	ms_status_len(int last_status)
{
	char	*s;
	size_t	len;

	s = ft_itoa(last_status);
	if (!s)
		return (0);
	len = ft_strlen(s);
	free(s);
	return (len);
}

static size_t	ms_var_value_len(const char *src, int *i, char **envp)
{
	int		len;
	char	*val;

	(*i)++;
	len = ms_var_name_len(src + *i);
	val = ms_get_env_value(src + *i, len, envp);
	*i += len;
	if (!val)
		return (0);
	return (ft_strlen(val));
}

static size_t	ms_dollar_len(const char *src, int *i, char **envp, int st)
{
	if (src[*i + 1] == '?')
	{
		*i += 2;
		return (ms_status_len(st));
	}
	if (ms_is_var_start(src[*i + 1]))
		return (ms_var_value_len(src, i, envp));
	(*i)++;
	return (1);
}

size_t	ms_expanded_len(const char *src, char **envp, int last_status)
{
	size_t	len;
	int		i;
	char	q;

	len = 0;
	i = 0;
	q = 0;
	while (src && src[i])
	{
		if (!q && (src[i] == '\'' || src[i] == '"'))
			q = src[i++];
		else if (q && src[i] == q)
		{
			q = 0;
			i++;
		}
		else if (src[i] == '$' && q != '\'')
			len += ms_dollar_len(src, &i, envp, last_status);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
