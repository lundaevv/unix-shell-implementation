# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/21 14:40:59 by lundaevv          #+#    #+#              #
#    Updated: 2026/01/22 19:31:35 by vlundaev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -------------------------------- Toolchain --------------------------------- #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

# -------------------------------- Project ----------------------------------- #

NAME		= minishell

SRC_PATH	= src
OBJ_PATH	= obj
INC_PATH	= includes

# -------------------------------- Libft ------------------------------------- #

LIBFT_DIR	= libft
LIBFT_A		= $(LIBFT_DIR)/libft.a

INCS		= -I $(INC_PATH) -I $(LIBFT_DIR)/includes

# -------------------------------- Readline ---------------------------------- #

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	RL_INC = -I /opt/homebrew/opt/readline/include
	RL_LIB = -L /opt/homebrew/opt/readline/lib -lreadline
else
	RL_INC =
	RL_LIB = -lreadline -lncurses
endif

# -------------------------------- Sources ----------------------------------- #

SRC = \
	main.c \
	loop.c \
	core/loop_utils.c \
	core/line_runner.c \
	core/line_steps.c \
	core/line_quotes.c \
	parsing/lexer/lexer.c \
	parsing/lexer/lexer_word.c \
	parsing/lexer/lexer_operator.c \
	parsing/lexer/lexer_utils.c \
	parsing/expander/expander.c \
	parsing/expander/expander_limiter.c \
	parsing/expander/expander_len.c \
	parsing/expander/expander_utils.c \
	parsing/expander/expander_quotes.c \
	parsing/expander/expander_unquote.c \
	parsing/expander/expander_unquote_run.c \
	parsing/syntax/parser_syntax.c \
	parsing/parser/parser.c \
	parsing/parser/parser_utils.c \
	parsing/parser/parser_build_cmds.c \
	parsing/parser/parser_free.c \
	parsing/parser/argv/parser_argv.c \
	parsing/parser/argv/parser_argv_count.c \
	parsing/parser/argv/parser_argv_utils.c \
	parsing/parser/redir/parser_redir_count.c \
	parsing/parser/redir/parser_redir_init.c \
	parsing/parser/redir/parser_redir_utils.c \
	parsing/parser/redir/parser_redir_map.c \
	parsing/parser/redir/parser_redir_build.c \
	exec/exec_pipeline.c \
	exec/exec_pipeline_single.c \
	exec/exec_pipeline_multi.c \
	exec/exec_pipes.c\
	exec/exec_stdio.c \
	exec/exec_wait.c \
	exec/exec_cmd.c \
	exec/exec_cmd_path.c \
	exec/path_resolve.c \
	exec/exec_utils.c \
	exec/exec_signals.c \
	exec/exec_signals_heredoc.c \
	exec/exec_redirs.c \
	exec/exec_redirs_apply.c \
	exec/exec_child.c \
	exec/exec_heredoc.c \
	exec/exec_heredoc_utils.c \
	exec/exec_heredoc_prepare.c \
	builtin/builtin.c \
	builtin/builtin_echo.c \
	builtin/builtin_cd.c \
	builtin/builtin_pwd_env.c \
	builtin/builtin_export_unset.c \
	builtin/builtin_exit.c \
	builtin/exit_atoll.c \
	builtin/exit_num_utils.c \
	builtin/builtin_ident.c\
	env/env.c \
	env/env_utils.c \
	env/env_append.c \
	env/env_export_print.c \

SRCS = $(addprefix $(SRC_PATH)/, $(SRC))
OBJS = $(addprefix $(OBJ_PATH)/, $(SRC:.c=.o))

HDRS = \
	$(INC_PATH)/minishell.h \
	$(INC_PATH)/parsing.h \
	$(INC_PATH)/execution.h \
	$(INC_PATH)/ms_errors.h \
	$(LIBFT_DIR)/includes/libft.h \
	$(LIBFT_DIR)/includes/ft_printf.h \
	$(LIBFT_DIR)/includes/get_next_line.h

# -------------------------------- Rules ------------------------------------- #

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "[LD] $(NAME)"
	@$(CC) $(CFLAGS) $(OBJS) -L $(LIBFT_DIR) -lft $(RL_LIB) -o $(NAME)
	@echo "[OK] built -> $(NAME)"

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

libft:
	@$(MAKE) -C $(LIBFT_DIR)

relibft:
	@$(MAKE) -C $(LIBFT_DIR) re

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(HDRS)
	@mkdir -p $(@D)
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) $(INCS) $(RL_INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_PATH)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re libft relibft
