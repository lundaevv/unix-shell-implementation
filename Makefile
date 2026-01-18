NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -Ilibft
LDFLAGS = -lreadline -lncurses


LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = \
src/main.c \
src/exec/exec_pipeline.c \
src/exec/exec_pipeline_single.c\
src/exec/exec_pipeline_multi.c \
src/exec/exec_stdio.c\
src/exec/exec_wait.c\
src/exec/exec_cmd.c \
src/exec/path_resolve.c \
src/exec/exec_utils.c \
src/exec/exec_signals.c\
src/exec/exec_redirs.c\
src/exec/exec_child.c\
src/builtin/builtin.c \
src/builtin/builtin_echo.c \
src/builtin/builtin_cd.c \
src/builtin/builtin_pwd_env.c \
src/builtin/builtin_export_unset.c \
src/builtin/builtin_exit.c \
src/env/env.c \
src/env/env_utils.c \
src/exec/exec_heredoc.c \
src/parser/parse_stub.c \


OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)


$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
