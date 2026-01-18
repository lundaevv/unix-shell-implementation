#include "minishell.h"

/*
** Export one argument.
** Supported:
** - KEY=VALUE
** - KEY          (sets KEY to empty string, minimal behavior)
*/
static int	export_one(t_shell *sh, char *arg)
{
	char	*eq;
	char	*name;
	int		ret;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (env_set(sh, arg, ""));
	name = ft_substr(arg, 0, (size_t)(eq - arg));
	if (!name)
		return (1);
	ret = env_set(sh, name, eq + 1);
	free(name);
	return (ret);
}

/* minimal: no printing/sorting yet */
int	bi_export(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->argv || !cmd->argv[1])
		return (0);
	i = 1;
	ret = 0;
	while (cmd->argv[i])
	{
		if (export_one(sh, cmd->argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}

int	bi_unset(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->argv || !cmd->argv[1])
		return (0);
	i = 1;
	ret = 0;
	while (cmd->argv[i])
	{
		if (env_unset(sh, cmd->argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
