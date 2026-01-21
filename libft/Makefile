# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/13 03:11:33 by vlundaev          #+#    #+#              #
#    Updated: 2026/01/21 15:19:11 by vlundaev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# ----------------------------- LIBFT MAKEFILE ------------------------------- #

# Toolchain
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
AR		= ar
ARFLAGS	= rcs

# Output
NAME	= libft.a

# Layout
SRC_DIR	= src
OBJ_DIR	= obj
INC_DIR	= includes
INCS	= -I $(INC_DIR)

# Headers (kept separate, all in libft/includes/)
HDRS     = $(INC_DIR)/libft.h			\
           $(INC_DIR)/ft_printf.h		\
           $(INC_DIR)/get_next_line.h


# ----------------------- File lists (edit as needed) ------------------------ #

CSTD	= 						\
		ft_atoi.c				\
		ft_bzero.c				\
		ft_calloc.c				\
		ft_isalnum.c			\
		ft_isalpha.c			\
		ft_isascii.c			\
		ft_isdigit.c			\
		ft_isprint.c			\
		ft_itoa.c				\
		ft_memchr.c				\
		ft_memcmp.c				\
		ft_memcpy.c				\
		ft_memmove.c			\
		ft_memset.c				\
		ft_putchar_fd.c			\
		ft_putendl_fd.c			\
		ft_putnbr_fd.c			\
		ft_putstr_fd.c			\
		ft_split.c				\
		ft_strchr.c				\
		ft_strdup.c				\
		ft_striteri.c			\
		ft_strjoin.c			\
		ft_strlcat.c 			\
		ft_strlcpy.c			\
		ft_strlen.c 			\
		ft_strmapi.c			\
		ft_strncmp.c			\
		ft_strnstr.c			\
		ft_strrchr.c			\
		ft_strtrim.c			\
		ft_substr.c				\
		ft_tolower.c			\
		ft_toupper.c

PRINTF	= 						\
		ft_printf.c				\
		ft_printf_utils.c

GNL		= 						\
		get_next_line.c			\

# Map sources to subdirs
SRC		=								\
		$(addprefix cstd/, $(CSTD))		\
		$(addprefix printf/, $(PRINTF))	\
		$(addprefix gnl/, $(GNL))

SRCS	= $(addprefix $(SRC_DIR)/, $(SRC))
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Grouped object paths (handy for group targets)
OBJS_CSTD	= $(addprefix $(OBJ_DIR)/cstd/, $(CSTD:.c=.o))
OBJS_PRINTF	= $(addprefix $(OBJ_DIR)/printf/, $(PRINTF:.c=.o))
OBJS_GNL	= $(addprefix $(OBJ_DIR)/gnl/, $(GNL:.c=.o))


# ------------------------------ Rules --------------------------------------- #

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo "[AR] $(NAME)"
	@$(AR) $(ARFLAGS) $(NAME) $(OBJS)
	@printf "[OK] lib built -> %s\n" "$(NAME)"
	@printf "      cstd=%d  printf=%d  gnl=%d\n" \
	        $(words $(CSTD)) $(words $(PRINTF)) $(words $(GNL))

# Group builds (optional to call manually)
cstd: $(OBJ_DIR) $(OBJS_CSTD)
	@printf "[OK] cstd objects ready -> %d files\n" $(words $(CSTD))

printf: $(OBJ_DIR) $(OBJS_PRINTF)
	@printf "[OK] printf objects ready -> %d files\n" $(words $(PRINTF))

gnl: $(OBJ_DIR) $(OBJS_GNL)
	@printf "[OK] gnl objects ready -> %d files\n" $(words $(GNL))

# Create object directories quietly
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)	\
		$(OBJ_DIR)/cstd		\
		$(OBJ_DIR)/printf	\
		$(OBJ_DIR)/gnl

# Compile rule (shows file being compiled)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS)
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	@echo "[CLEAN] removing objects"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "[FCLEAN] removing $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re cstd printf gnl
