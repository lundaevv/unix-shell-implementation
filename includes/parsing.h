/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:44:33 by lundaevv          #+#    #+#             */
/*   Updated: 2025/12/18 00:34:49 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdlib.h>
# include <stdbool.h>

/*
** =========================
** PARSER -> EXEC CONTRACT
** =========================
**
** t_cmd:
** - argv is a NULL-terminated array of strings.
** - argv may contain empty strings (e.g. "" or '').
** - If argv != NULL, then argv[argc] == NULL terminator is guaranteed.
**
** - redirs is either:
**     - NULL when redir_count == 0
**     - a malloc'ed array of size redir_count when redir_count > 0
** - Each redirs[i].target is malloc'ed and owned by the parser.
**
** t_pipeline:
** - cmds is a malloc'ed array of size cmd_count 
** 				(cmd_count >= 1 when parse succeeds).
** - All memory returned by parser is freed by free_pipeline().
*/

/*
** =========================
** Token layer (lexer output)
** =========================
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/*
** =========================
** Parser output (execution input)
** =========================
*/
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
	int		redir_count;
}	t_cmd;

typedef struct s_pipeline
{
	t_cmd	*cmds;
	int		cmd_count;
}	t_pipeline;

/* =============================== LEXER ==================================== */

t_token		*lexer_tokenize(const char *line);
t_token		*lexer_read_operator(const char *line, int *i);
t_token		*lexer_read_word(const char *line, int *i);

t_token		*token_new(char *value, t_token_type type);
void		token_add_back(t_token **list, t_token *new_node);
void		token_list_print(t_token *list);
void		token_list_clear(t_token **list);
int			is_space(char c);

/* ============================== EXPANDER ================================== */

int			expand_tokens(t_token *list, char **envp, int last_exit_status);

char		*ms_expand_unquote(const char *src, char **envp, int last_status);
int			ms_expand_run(char *dst, const char *src, void **ctx);
size_t		ms_expanded_len(const char *src, char **envp, int last_status);

int			ms_is_var_start(char c);
int			ms_is_var_char(char c);
int			ms_var_name_len(const char *s);
char		*ms_get_env_value(const char *name, int len, char **envp);

/* =============================== SYNTAX =================================== */

int			validate_syntax(t_token *token);

/* =============================== PARSER =================================== */

t_pipeline	*parse_pipeline(t_token *tokens);
int			build_pipeline_cmds(t_pipeline *p, t_token *tokens);

int			count_commands(t_token *tokens);
void		init_cmd_array(t_cmd *cmds, int count);
void		free_pipeline(t_pipeline *p);

/* argv */
char		**build_argv_simple(t_token *tokens);
int			count_words_simple(t_token *tokens);
int			count_words_no_redirs(t_token *tokens);
int			free_argv_partial(char **argv, int filled);
void		skip_redir_pair(t_token **cur);

/* redirs */
int			is_redir_token(t_token_type type);
int			count_redirs_simple(t_token *tokens);
int			token_to_redir_type(t_token_type t, t_redir_type *out);
t_redir		*build_redirs_simple(t_token *tokens, int *out_count);

/* =============================== DEBUG ==================================== */
/*
** ВАЖНО:
** parsing.h не знает о t_shell.
** Debug, который печатает shell, объявляется в minishell.h.
*/
void		ms_debug_counts(t_token *tokens);
void		ms_debug_pipeline(const t_pipeline *p);

#endif