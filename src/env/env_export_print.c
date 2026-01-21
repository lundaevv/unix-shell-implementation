#include "minishell.h"

static int	env_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static int	ms_strcmp(const char *a, const char *b)
{
	int	i;

	i = 0;
	while (a[i] && b[i] && a[i] == b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

static void	sort_env(char **arr, int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < n - 1)
	{
		j = i + 1;
		while (j < n)
		{
			if (ms_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(const char *s)
{
	int	i;

	write(STDOUT_FILENO, "declare -x ", 11);
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	write(STDOUT_FILENO, s, i);
	if (s[i] == '=')
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, s + i + 1, ft_strlen(s + i + 1));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

void	export_print(char **envp)
{
	char	**arr;
	int		n;
	int		i;

	n = env_count(envp);
	arr = (char **)malloc(sizeof(char *) * (n + 1));
	if (!arr)
		return ;
	i = 0;
	while (i < n)
	{
		arr[i] = envp[i];
		i++;
	}
	arr[i] = NULL;
	sort_env(arr, n);
	i = 0;
	while (i < n)
	{
		print_export_line(arr[i]);
		i++;
	}
	free(arr);
}
