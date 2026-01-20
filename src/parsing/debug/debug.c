/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 19:18:50 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/20 15:40:10 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*redir_name(t_redir_type t)
{
	if (t == REDIR_IN)
		return ("<");
	if (t == REDIR_OUT)
		return (">");
	if (t == REDIR_APPEND)
		return (">>");
	if (t == REDIR_HEREDOC)
		return ("<<");
	return ("?");
}

static const char	*tok_name(t_token_type t)
{
	if (t == TOKEN_WORD)
		return ("WORD");
	if (t == TOKEN_PIPE)
		return ("PIPE");
	if (t == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (t == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (t == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (t == TOKEN_HEREDOC)
		return ("HEREDOC");
	return ("?");
}

static int	argv_len(char *const *argv)
{
	int	i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
		i++;
	return (i);
}

static void	ms_debug_redirs(const t_cmd *cmd)
{
	int	i;

	printf("  redirs ptr     = %p\n", (void *)cmd->redirs);
	printf("  redir_count    = %d\n", cmd->redir_count);
	if (!cmd->redirs || cmd->redir_count <= 0)
		return ;
	i = 0;
	while (i < cmd->redir_count)
	{
		printf("    redir[%d] ptr  = %p\n", i, (void *)&cmd->redirs[i]);
		printf("      op          = %s\n", redir_name(cmd->redirs[i].type));
		printf("      target ptr  = %p\n", (void *)cmd->redirs[i].target);
		printf("      target      = '%s'\n",
			(cmd->redirs[i].target ? cmd->redirs[i].target : "(null)"));
		printf("      heredoc_expand = %d\n", cmd->redirs[i].heredoc_expand);
		if (cmd->redirs[i].type == REDIR_HEREDOC)
			printf("      expand      = %d\n",
				(int)cmd->redirs[i].heredoc_expand);
		i++;
	}
}

/*
** Debug print of tokens (source)
*/
void	token_list_print(t_token *list)
{
	int	i;

	i = 0;
	while (list && i < 200)
	{
		printf("TOK[%d] node_ptr=%p type=%s(%d) value_ptr=%p",
			i, (void *)list, tok_name(list->type), list->type,
			(void *)list->value);
		if (list->type == TOKEN_WORD)
			printf(" has_quotes=%d", (int)list->has_quotes);
		printf(" value='%s'\n", (list->value ? list->value : "(null)"));
		list = list->next;
		i++;
	}
	if (i == 200)
		printf("(stop: token list too long)\n");
}

static void	ms_debug_line(const char *line)
{
	if (!line)
		return ;
	ft_putstr_fd("You typed: ", STDOUT_FILENO);
	ft_putendl_fd((char *)line, STDOUT_FILENO);
}

static void	ms_debug_tokens(t_token *tokens)
{
	if (!tokens)
		return ;
	printf("=== TOKENS (after expand / before parse) ===\n");
	token_list_print(tokens);
}

static void	ms_debug_argv(const t_cmd *cmd)
{
	int	argc;
	int	i;

	printf("argv ptr       = %p\n", (void *)cmd->argv);
	if (!cmd->argv)
	{
		printf("argc           = 0\n");
		printf("  (argv is NULL)\n");
		return ;
	}
	argc = argv_len(cmd->argv);
	printf("argc           = %d\n", argc);
	i = 0;
	while (cmd->argv[i] && i < 50)
	{
		printf("  argv[%d] ptr  = %p\n", i, (void *)cmd->argv[i]);
		printf("  argv[%d]      = '%s'\n", i, cmd->argv[i]);
		i++;
	}
	if (i == 50 && cmd->argv[i] != NULL)
		printf("  (stop: argv not NULL-terminated?)\n");
	printf("  argv[%d] ptr  = %p (terminator)\n",
		argc, (void *)cmd->argv[argc]);
}

/*
** Print argv + redirs for each command in the pipeline (what exec will use).
** Limit argv to 50 args to avoid infinite loops if argv is not NULL-terminated.
*/
void	ms_debug_pipeline(const t_pipeline *p)
{
	int	j;

	if (!p)
	{
		printf("=== PIPELINE (NULL) ===\n");
		return ;
	}
	printf("=== PIPELINE STRUCTURE SENT TO EXEC ===\n");
	printf("pipeline ptr   = %p\n", (void *)p);
	printf("cmds ptr       = %p\n", (void *)p->cmds);
	printf("cmd_count      = %d\n", p->cmd_count);
	if (!p->cmds || p->cmd_count <= 0)
		return ;
	j = 0;
	while (j < p->cmd_count)
	{
		printf("\n--- CMD[%d] ---\n", j);
		printf("cmd ptr        = %p\n", (void *)&p->cmds[j]);
		ms_debug_argv(&p->cmds[j]);
		printf("redirs:\n");
		ms_debug_redirs(&p->cmds[j]);
		j++;
	}
	printf("\n=== END PIPELINE ===\n");
}

/*
** Global debug entry point. Call this once per line.
*/
void	ms_debug_state(t_shell *shell, const char *line, t_token *tokens,
		t_pipeline *p)
{
	(void)shell;
	printf("\n================ DEBUG STATE ================\n");
	ms_debug_line(line);
	ms_debug_tokens(tokens);
	ms_debug_pipeline(p);
	printf("=============================================\n\n");
}

void	ms_debug_counts(t_token *tokens)
{
	printf("DEBUG words_no_redirs=%d\n", count_words_no_redirs(tokens));
	printf("DEBUG redirs=%d\n", count_redirs_simple(tokens));
}
