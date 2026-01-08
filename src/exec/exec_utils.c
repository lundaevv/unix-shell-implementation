#include "minishell.h"

char	*get_env_value(char *name, char **envp)
{
	size_t	len;
	int		i;

	len = strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

