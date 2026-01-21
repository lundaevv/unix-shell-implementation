#include "minishell.h"

int	bi_pwd(void)
{
	char	buf[4096];

	if (!getcwd(buf, sizeof(buf)))
	{
		perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, buf, ft_strlen(buf));
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	bi_env(t_shell *sh)
{
	if (!sh)
		return (1);
	env_print(sh->envp);
	return (0);
}

