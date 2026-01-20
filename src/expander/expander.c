#include "minishell.h"

/*
** Expand all tokens before parsing.
** "$?" and VAR in WORD tokens.
*/
int	expand_tokens(t_token *list, char **envp, int last_exit_status)
{
	char	*new_value;

	while (list)
	{
		if (list->type == TOKEN_WORD)
		{
			new_value = ms_expand_unquote(list->value, envp, last_exit_status);
			if (!new_value)
				return (1);
			free(list->value);
			list->value = new_value;
		}
		list = list->next;
	}
	return (0);
}
