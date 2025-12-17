/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:52:15 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/17 23:11:31 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "libft.h"
# include "ft_printf.h"
# include "parsing.h"

/*
** =========================
** Core shell state
** =========================
*/
typedef struct s_shell
{
	int		exit_status;
	char	**env;
}	t_shell;

/*
** =========================
** Shell loop / line runner
** =========================
*/
void	shell_loop(t_shell *shell);
int		run_line(t_shell *shell, char *line);
int		line_build_state(t_shell *shell, char *line,
			t_token **out_tokens, t_pipeline **out_p);

/*
** =========================
** Loop utils
** =========================
*/
int		is_exit_command(const char *line);
int		handle_history_and_exit(t_shell *shell, char *line);
int		handle_expand_error(t_token **tokens, char *line);

/*
** =========================
** Debug (core-level)
** =========================
*/
void	ms_debug_state(t_shell *shell, const char *line,
			t_token *tokens, t_pipeline *p);

#endif