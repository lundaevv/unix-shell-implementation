#include "minishell.h"

static int	is_echo_n(const char *s)
{
	if (!s)
		return (0);
	return (ft_strncmp(s, "-n", 3) == 0);
}

int	bi_echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (!cmd || !cmd->argv)
		return (0);
	if (is_echo_n(cmd->argv[1]))
	{
		newline = 0;
		i = 2;
	}
	while (cmd->argv[i])
	{
		write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
		if (cmd->argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
