/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:04:51 by gperedny          #+#    #+#             */
/*   Updated: 2026/01/08 02:48:32 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*includes*/
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
#include <errno.h>
# include <signal.h>
# include <string.h>
#include "libft.h"
#include <fcntl.h>

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
	char				*filename;
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
extern int g_signal;

/*execution*/
int exec_pipeline(t_shell *sh, t_pipeline *p);
void exec_cmd(t_cmd *cmd, char **envp);

/*path utils*/
char *resolve_path(const char *cmd, char **envp);
char *get_env_value(char *name, char **envp);
void free_split(char **arr);
char **ft_split(char const *s, char c);

/*redirections*/
int		apply_redirections(t_cmd *cmd);

/*pipes*/
void child_run_pipeline_cmd(t_shell *sh, t_pipeline *p, int i, int pipes[][2]);

/* builtins */
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

/* env helpers */
char **env_copy(char **envp);
char *env_get(char **envp, const char *name);
int   env_set(t_shell *sh, const char *name, const char *value);  // export
int   env_unset(t_shell *sh, const char *name);
void  env_print(char **envp);           // env
void  export_print(char **envp);        // export (no args)


#endif
