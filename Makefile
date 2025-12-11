# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/08 11:55:51 by vlundaev          #+#    #+#              #
#    Updated: 2025/12/11 19:31:59 by vlundaev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --------------------------------  MAKEFILE --------------------------------- #

# Toolchain
CC			= cc
CFLAGS		= -Wall -Wextra -Werror

# Output
NAME		= minishell

# Layout
SRC_PATH	= src/
OBJ_PATH	= obj/
INC_PATH	= includes/

# Libft
LIBFT_DIR	= libft
LIBFT_A		= $(LIBFT_DIR)/libft.a

# Includes (project + libft)
INCS		= -I $(INC_PATH) -I $(LIBFT_DIR)/includes

# Readline (Linux default). For macOS, uncomment two lines below.
RL_INC      =
RL_LIB      = -lreadline

# macOS:
#RL_INC    = -I /opt/homebrew/opt/readline/include
#RL_LIB    = -L /opt/homebrew/opt/readline/lib -lreadline

# Sources
SRC			= 						\
		main.c						\
		loop.c						\
		loop_utils.c				\
		parsing/debug.c				\
		parsing/lexer.c				\
		parsing/lexer_utils.c		\
		parsing/lexer_operator.c	\
		parsing/lexer_word.c		\
		parsing/expander.c			\
		parsing/expander_var.c		\
		parsing/parser.c			\
		parsing/parser_utils.c		\
		parsing/parser_free.c

SRCS		= $(addprefix $(SRC_PATH), $(SRC))
OBJS		= $(addprefix $(OBJ_PATH), $(SRC:.c=.o))

HDRS		=									\
		$(INC_PATH)minishell.h					\
		$(LIBFT_DIR)/includes/libft.h			\
		$(LIBFT_DIR)/includes/ft_printf.h		\
		$(LIBFT_DIR)/includes/get_next_line.h	


# ------------------------------ Rules --------------------------------------- #

all: $(NAME)

$(NAME): $(OBJ_PATH) $(OBJS) $(LIBFT_A)
	@echo "[LD] $(NAME)"
	@$(CC) $(CFLAGS) $(OBJS) -L $(LIBFT_DIR) -lft \
		$(RL_LIB) -o $(NAME)
	@echo "[OK] built -> $(NAME)"

# Build libft automatically when the archive is required
$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

# Build libft
libft:
	@$(MAKE) -C $(LIBFT_DIR)

# Force rebuild of libft if needed
relibft:
	@$(MAKE) -C $(LIBFT_DIR) re

# Ensure object dir exists
$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(OBJ_PATH)/parsing
	@mkdir -p $(OBJ_PATH)/exec
	@mkdir -p $(OBJ_PATH)/builtins

# Compile objects (both project and libft headers; add RL_INC for completeness)
$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HDRS) | $(OBJ_PATH)
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) $(INCS) $(RL_INC) -c $< -o $@

clean:
	@echo "[CLEAN] objects"
	@rm -rf $(OBJ_PATH)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "[FCLEAN] $(NAME)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re libft relibft

# .SECONDARY: here acts only as a safety net — just in case some rule chain
# ever makes an object (or other file) intermediate.
.SECONDARY: