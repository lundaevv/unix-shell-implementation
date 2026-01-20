#ifndef MS_ERRORS_H
# define MS_ERRORS_H

/*
** Minishell exit status codes (bash-compatible)
*/

# define MS_SUCCESS 0

# define MS_GENERAL_ERROR 1
# define MS_SYNTAX_ERROR 2

# define MS_CMD_NOT_FOUND 127
# define MS_CMD_NOT_EXECUTABLE 126

# define MS_SIGINT 130
# define MS_SIGQUIT 131

# define MS_HEREDOC_ABORT 130

#endif
