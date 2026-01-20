

#ifndef MINISHELL_H
# define MINISHELL_H

/*includes*/
#include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
#include <errno.h>
# include <signal.h>
# include <string.h>
#include <fcntl.h>
#include <limits.h>

/*redirections*/
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}				t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*target;
	int 				heredoc_expand;
}				t_redir;

/*command*/
typedef struct s_cmd
{
	char				**argv;
	t_redir				*redirs;
	int					redir_count;
}				t_cmd;

/*pipeline*/
typedef struct s_pipeline
{
	t_cmd				*cmds;
	int					count;
}				t_pipeline;

/*shell*/
typedef struct s_shell
{
    char **envp;
    int   exit_status;
	int   should_exit;
} t_shell;


/*globals*/
extern volatile sig_atomic_t g_signal;

/*signals*/
void signals_interactive(void);
void signals_parent_exec(void);
void signals_child_exec(void);

/*execution + helpers*/
int exec_pipeline(t_shell *sh, t_pipeline *p);
int exec_pipeline_single(t_shell *sh, t_cmd *cmd);
int exec_pipeline_multi(t_shell *sh, t_pipeline *p);
int ms_wait_last(pid_t *pids, int n);
int ms_status_to_exit(int status);
int ms_stdio_save(int *in, int *out);
void ms_stdio_restore(int in, int out);
void ms_close_all_pipes(int pipes[][2], int n);
void exec_cmd(t_cmd *cmd, char **envp);

/*path utils*/
char *resolve_path(const char *cmd, char **envp);
char *get_env_value(char *name, char **envp);
void free_split(char **arr);
char **ft_split(char const *s, char c);

/*redirections*/
int	apply_redirections(t_shell *sh, t_cmd *cmd);

/*heredoc*/
int 	heredoc_open(t_shell *sh, const char *limiter, int expander);

/*expander + utils(from parter)*/
char    *ms_expand_unquote(const char *src, char **envp, int last_status);
size_t  ms_expanded_len(const char *src, char **envp, int last_status);
int     ms_expand_run(char *dst, const char *src, void **ctx);
int     ms_is_var_start(char c);
int     ms_is_var_char(char c);
int     ms_var_name_len(const char *s);
char    *ms_get_env_value(const char *name, int len, char **envp);

/*pipes*/
void child_run_pipeline_cmd(t_shell *sh, t_pipeline *p, int i, int pipes[][2]);

/* builtins + utils */
int  is_builtin(const char *name);
int  is_parent_builtin(const char *name);
int  run_builtin(t_shell *sh, t_cmd *cmd);
int  bi_echo(t_cmd *cmd);
int  bi_pwd(void);
int  bi_env(t_shell *sh);
int  bi_cd(t_shell *sh, t_cmd *cmd);
int  bi_export(t_shell *sh, t_cmd *cmd);
int  bi_unset(t_shell *sh, t_cmd *cmd);
int  bi_exit(t_shell *sh, t_cmd *cmd);
int		ms_is_strict_number(const char *s);
int		ms_atoll_checked(const char *s, long long *out);
int ms_is_digit(char c);
int ms_is_space(char c);

/* env helpers */
char **env_copy(char **envp);
char *env_get(char **envp, const char *name);
int   env_set(t_shell *sh, const char *name, const char *value);  // export
int   env_unset(t_shell *sh, const char *name);
void  env_print(char **envp);           // env
void  export_print(char **envp);        // export (no args)

/*parse*/
int  parse_stub(char *line, t_pipeline *p);
void free_pipeline_stub(t_pipeline *p);


#endif
