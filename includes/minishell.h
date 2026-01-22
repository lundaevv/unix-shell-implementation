/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:26:56 by lundaevv          #+#    #+#             */
/*   Updated: 2026/01/22 16:20:43 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* --------------------------------- C libs --------------------------------- */

# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <limits.h>

# include <sys/types.h>
# include <sys/stat.h>

/* ------------------------------- Readline --------------------------------- */

# include <readline/readline.h>
# include <readline/history.h>

/* -------------------------------- Project --------------------------------- */

# include "libft.h"
# include "ft_printf.h"

# include "ms_errors.h"
# include "parsing.h"
# include "execution.h"

/*
** =============================================================================
** Core shell state (SINGLE SOURCE OF TRUTH)
** =============================================================================
*/
typedef struct s_shell
{
	char	**envp;
	int		exit_status;
	int		should_exit;
}	t_shell;

/*
** =============================================================================
** Shell loop
** =============================================================================
*/
void	shell_loop(t_shell *shell);

/*
** =============================================================================
** Loop utils
** =============================================================================
*/
int		handle_history_and_exit(t_shell *shell, char *line);
int		handle_expand_error(t_token **tokens, char *line);

#endif