/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:52:15 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 19:54:25 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>

# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

# include "libft.h"
# include "ft_printf.h"

// Main shell state structure
// More fields will be added later (env list, tokens, commands, etc.)
typedef struct s_shell
{
	int		exit_status;
	char	**env; //t_env *env (instead) / now its just a copy of the envp
}	t_shell;

// Token types produced by the lexer
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

// Single token in the lexer linked list
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

// new struct for env
/*
typedef struct s_env
{
	char			*name
	char			*value;
	struct s_env	*next;
}	t_env;
*/

/* 
** Redirection type for a command:
** <, >, >>, <<
*/
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

/*
** Single redirections:
** - type	- which operator (<, >, >>, <<)
** - target - filename or heredoc limiter
*/
typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
}	t_redir;

/*
** Sinle command in a pipeline:
** - argv			- NULL-terminated array of arguments
** - redirs			- array of redirections (can be NULL)
** - redir_count	- number of elements in redirs
*/
typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
	int		redir_count;
}	t_cmd;

/*
** Full pipeline (a line possibly containing pipes):
** - cmds		- array of commands
** - cmd_count	- number of commands in the pipeline
*/
typedef struct s_pipeline
{
	t_cmd	*cmds;
	int		cmd_count;
}	t_pipeline;

// Main minishell loop
void		shell_loop(t_shell *shell);

// Lop utils:
int			is_exit_command(const char *line);
int			handle_history_and_exit(t_shell *shell, char *line);
int			handle_expand_error(t_token **tokens, char *line);

// Lexer main: split an anput line into a linked list of tokens
t_token		*lexer_tokenize(const char *line);

// Token utils:
t_token		*token_new(char *value, t_token_type type);
void		token_add_back(t_token **list, t_token *new_node);
void		token_list_print(t_token *list);
void		token_list_clear(t_token **list);
int			is_space(char c);

// Lexer helpers:
t_token		*lexer_read_operator(const char *line, int *i);
t_token		*lexer_read_word(const char *line, int *i);

// Expander
int			expand_tokens(t_token *list, char **envp, int last_status);
char		*expand_variables_in_value(const char *value, char **envp);

// Parser
t_pipeline	*parse_pipeline(t_token *tokens);

// Parser utils
int			count_commands(t_token *tokens);
void		init_cmd_array(t_cmd *cmds, int count);
int			count_words_simple(t_token *tokens);
void		free_pipeline(t_pipeline *p);

// DEBUG
void		ms_debug_state(t_shell *shell, char *line,
				t_token *tokens, t_pipeline *p);

#endif