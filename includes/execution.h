/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 15:18:42 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 18:54:34 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

typedef struct s_shell			t_shell;

/* globals */
extern volatile sig_atomic_t	g_signal;

/* signals */
void							signals_interactive(void);
void							signals_parent_exec(void);
void							signals_child_exec(void);
void							signals_heredoc(void);

/* execution */
int								exec_pipeline(t_shell *sh, t_pipeline *p);
int								exec_pipeline_single(t_shell *sh, t_cmd *cmd);
int								exec_pipeline_multi(t_shell *sh, t_pipeline *p);
void							ms_close_all_pipes(int pipes[][2], int n);
int								ms_make_pipes(int pipes[][2], int n);
int								ms_wait_last(pid_t *pids, int n);
int								ms_status_to_exit(int status);
int								ms_stdio_save(int *in, int *out);
void							ms_stdio_restore(int in, int out);
void							ms_close_all_pipes(int pipes[][2], int n);
int								ms_cmd_is_empty(t_cmd *cmd);
void							exec_cmd(t_cmd *cmd, char **envp);
int								ms_exec_error_code(void);
void							ms_exec_with_path(t_cmd *cmd, char **envp);

/* path utils */
char							*resolve_path(const char *cmd, char **envp);
char							*get_env_value(char *name, char **envp);
void							free_split(char **arr);

/* redirections */
int								apply_redirections(t_shell *sh, t_cmd *cmd);
int								apply_one_redir(t_shell *sh, t_redir *r);

/* heredoc */
int								heredoc_open(t_shell *sh, const char *limiter,
									int expander);
char							*hd_str_add_char(char *s, char c);
int								hd_write_line(int wfd, char *line);
int								prepare_heredocs(t_shell *sh, t_cmd *cmd);

/* pipes */
void							child_run_pipeline_cmd(t_shell *sh,
									t_pipeline *p, int i, int pipes[][2]);

/* builtins */
int								is_builtin(const char *name);
int								is_parent_builtin(const char *name);
int								run_builtin(t_shell *sh, t_cmd *cmd);

int								bi_echo(t_cmd *cmd);
int								bi_pwd(void);
int								bi_env(t_shell *sh);
int								bi_cd(t_shell *sh, t_cmd *cmd);
int								bi_export(t_shell *sh, t_cmd *cmd);
int								bi_unset(t_shell *sh, t_cmd *cmd);
int								bi_exit(t_shell *sh, t_cmd *cmd);

int								ms_is_strict_number(const char *s);
int								ms_atoll_checked(const char *s, long long *out);
int								ms_is_digit(char c);
int								ms_is_space(char c);
int								ms_is_valid_ident(const char *s);
void							ms_err_ident(const char *cmd, const char *arg);

/* env helpers */
char							**env_copy(char **envp);
char							*env_get(char **envp, const char *name);
int								env_set(t_shell *sh, const char *name,
									const char *value);
int								env_unset(t_shell *sh, const char *name);
void							env_print(char **envp);
void							export_print(char **envp);
int								env_append_kv(t_shell *sh, char *kv);

#endif
