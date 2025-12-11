/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:50:40 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 19:56:05 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Count arguments and malloc for argv
*/
static char	**alloc_argv_simple(t_token *tokens, int *count)
{
	char	**argv;

	*count = count_words_simple(tokens);
	argv = (char **)malloc(sizeof(char *) * ((size_t)(*count + 1)));
	if (!argv)
		return (NULL);
	return (argv);
}

/*
** Fills argv with strings
*/
static int	fill_argv_simple(char **argv, t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
		{
			argv[i] = ft_strdup(tokens->value);
			if (!argv[i])
			{
				while (i > 0)
				{
					i--;
					free(argv[i]);
				}
				return (1);
			}
			i++;
		}
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (0);
}

/*
** Combine 2: count and initialize the array + fill it
** returns argv
*/
static char	**build_argv_simple(t_token *tokens)
{
	char	**argv;
	int		count;

	argv = alloc_argv_simple(tokens, &count);
	if (!argv)
		return (NULL);
	if (fill_argv_simple(argv, tokens))
	{
		free(argv);
		return (NULL);
	}
	return (argv);
}

/*
** Alloc and init t_pipeline
*/
static t_pipeline	*alloc_pipeline_struct(int cmd_count)
{
	t_pipeline	*p;

	p = (t_pipeline *)malloc(sizeof(t_pipeline));
	if (!p)
		return (NULL);
	p->cmd_count = cmd_count;
	p->cmds = (t_cmd *)malloc(sizeof(t_cmd) * (size_t)cmd_count);
	if (!p->cmds)
	{
		free(p);
		return (NULL);
	}
	init_cmd_array(p->cmds, cmd_count);
	return (p);
}

/*
** Build a simple pipeline structure from a token list.
** For now:
**  - count commands separated by PIPE
**  - allocate t_pipeline and its cmds array
**  - do not fill argv or redirections yet
*/
t_pipeline	*parse_pipeline(t_token *tokens)
{
	t_pipeline	*p;
	int			cmd_count;

	if (!tokens)
		return (NULL);
	cmd_count = count_commands(tokens);
	p = alloc_pipeline_struct(cmd_count);
	if (!p)
		return (NULL);
	if (cmd_count == 1)
	{
		p->cmds[0].argv = build_argv_simple(tokens);
		if (!p->cmds[0].argv)
		{
			free(p->cmds);
			free(p);
			return (NULL);
		}
	}
	return (p);
}
