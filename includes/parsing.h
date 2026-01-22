/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:44:33 by lundaevv          #+#    #+#             */
/*   Updated: 2026/01/22 19:29:58 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/*
** ============================================================================
** PARSER -> EXEC CONTRACT
** ============================================================================
**
** Ownership / lifetime:
** - Parser allocates all memory inside t_pipeline/t_cmd:
**     - argv strings
**     - redirs targets
**     - argv/redirs arrays
** - Executor must NOT free individual fields.
** - Use free_pipeline() to free everything returned by the parser.
**
** Segmentation rules:
** - Each t_cmd is built from tokens up to (but not including) TOKEN_PIPE.
** - Redirections belong only to the command segment where they appear.
**
** t_cmd (argv):
** - argv is ALWAYS non-NULL on successful parse.
** - argv is a NULL-terminated array of malloc'ed strings.
** - Empty strings are valid arguments (e.g. "" or '').
** - For an empty command (no WORD tokens excluding redirs):
**     argv[0] == NULL
**
** t_cmd (redirs):
** - redirs follows a strict count/pointer contract:
**     - if redir_count == 0  -> redirs == NULL
**     - if redir_count  > 0  -> redirs points to a malloc'ed array
** - Each redirs[i].target is malloc'ed and owned by the parser.
**
** t_pipeline:
** - cmds is a malloc'ed array of size cmd_count.
** - On successful parse:
**     - cmd_count >= 1
**     - cmds != NULL
**
** Failure behavior:
** - On parse failure parse_pipeline() returns NULL.
** - Parser must not leak memory on failure.
**
** ============================================================================
** REDIRECTION SEMANTICS (executor rules)
** ============================================================================
**
** Redirs are applied from left to right.
** The LAST redirection of a given stream wins.
**
** Examples (stdin):
** - cmd << A << B       -> stdin is heredoc B (A is still read/consumed)
** - cmd < in << A       -> stdin is heredoc A
** - cmd << A < in       -> stdin is file "in"
**
** ============================================================================
** HEREDOC CONTRACT (<< limiter)
** ============================================================================
**
** Parser responsibilities:
** - For TOKEN_HEREDOC, parser creates one redirection:
**     redir.type   = REDIR_HEREDOC
**     redir.target = limiter string (malloc'ed, quotes removed)
**
** - redir.heredoc_expand is set based on limiter quoting:
**     - true  if limiter token had NO quotes
**     - false if limiter token contained ANY quotes (', ")
**
** - Quotes around limiter MUST be removed in redir.target.
**   Examples:
**     << EOF         -> target="EOF",    heredoc_expand=true
**     << 'EOF'       -> target="EOF",    heredoc_expand=false
**     << "$USER"     -> target="$USER",  heredoc_expand=false
**     << "E'O'F"     -> target="E'O'F",  heredoc_expand=false
**
** - Limiter itself is NEVER expanded by the parser.
**
** Executor responsibilities:
** - Executor must process heredocs in the SAME order as redirs[].
** - For each REDIR_HEREDOC:
**     - read lines until a line equals redir.target exactly
**     - if redir.heredoc_expand == true:
**         expand $VAR and $? inside heredoc body (bash-like)
**     - if redir.heredoc_expand == false:
**         heredoc body is literal (NO expansions)
**
** - If multiple heredocs exist in one command:
**     - executor must READ/CONSUME all of them
**     - stdin must be connected to the LAST heredoc after all redirs
**
** Signals:
** - If Ctrl-C happens while reading heredoc:
**     - stop heredoc reading immediately
**     - set last exit status to 130
**     - do NOT execute the pipeline/commands
**
** Memory:
** - All memory in redir.target is owned by the parser and freed by
**   free_pipeline().
*/

typedef struct s_shell	t_shell;

/*
** =============================================================================
** Token layer (lexer output)
** =============================================================================
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	bool				has_quotes;
	struct s_token		*next;
}						t_token;

/*
** =============================================================================
** Parser output (execution input)
** =============================================================================
*/
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*target;
	bool				heredoc_expand;
	int					hd_fd;
}						t_redir;

typedef struct s_cmd
{
	char				**argv;
	t_redir				*redirs;
	int					redir_count;
}						t_cmd;

typedef struct s_pipeline
{
	t_cmd				*cmds;
	int					cmd_count;
}						t_pipeline;

/* ============================ LINE RUNNER ================================= */

int						run_line(t_shell *shell, char *line);
int						line_build_state(t_shell *shell, char *line,
							t_token **out_tokens, t_pipeline **out_p);
int						handle_unclosed_quotes(t_shell *shell, char *line);
int						is_only_spaces(const char *s);

/* =============================== LEXER ==================================== */

t_token					*lexer_tokenize(const char *line);
t_token					*lexer_read_operator(const char *line, int *i);
t_token					*lexer_read_word(const char *line, int *i);

t_token					*token_new(char *value, t_token_type type);
void					token_add_back(t_token **list, t_token *new_node);
void					token_list_print(t_token *list);
void					token_list_clear(t_token **list);
int						is_space(char c);

/* ============================== EXPANDER ================================== */

int						expand_tokens(t_token *list, char **envp,
							int last_exit_status);

char					*ms_expand_unquote(const char *src, char **envp,
							int last_status);
int						ms_expand_run(char *dst, const char *src, void **ctx);
size_t					ms_expanded_len(const char *src, char **envp,
							int last_status);

char					*ms_unquote_limiter(const char *src);
int						ms_quote_step_i(const char *s, int *i, char *q);
int						ms_quote_step_p(const char **p, char *q);

int						ms_is_var_start(char c);
int						ms_is_var_char(char c);
int						ms_var_name_len(const char *s);
char					*ms_get_env_value(const char *name, int len,
							char **envp);

/* =============================== SYNTAX =================================== */

int						validate_syntax(t_token *token);

/* =============================== PARSER =================================== */

t_pipeline				*parse_pipeline(t_token *tokens);
int						build_pipeline_cmds(t_pipeline *p, t_token *tokens);

int						count_commands(t_token *tokens);
void					init_cmd_array(t_cmd *cmds, int count);
void					free_pipeline(t_pipeline *p);

/* argv */
char					**build_argv_simple(t_token *tokens);
int						count_words_simple(t_token *tokens);
int						count_words_no_redirs(t_token *tokens);
int						free_argv_partial(char **argv, int filled);
void					skip_redir_pair(t_token **cur);

/* redirs */
int						is_redir_token(t_token_type type);
int						count_redirs_simple(t_token *tokens);
int						token_to_redir_type(t_token_type t, t_redir_type *out);
t_redir					*build_redirs_simple(t_token *tokens, int *out_count);
void					init_redirs(t_redir *r, int count);

#endif