# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/18 16:24:24 by gperedny          #+#    #+#              #
#    Updated: 2025/07/09 13:11:19 by gperedny         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 	libft.a
CC= cc
CFLAGS = -Wall -Wextra -Werror
SRCS = \
ft_strjoin.c\
ft_memchr.c \
ft_substr.c \
ft_calloc.c \
ft_strnstr.c \
ft_memcmp.c \
ft_strchr.c \
ft_strrchr.c \
ft_tolower.c \
ft_toupper.c \
ft_strlcat.c \
ft_memmove.c \
ft_memcpy.c \
ft_bzero.c \
ft_memset.c \
ft_atoi.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isascii.c \
ft_isdigit.c \
ft_isprint.c \
ft_strdup.c \
ft_strlcpy.c \
ft_strlen.c \
ft_strncmp.c\
ft_striteri.c\
ft_strmapi.c\
ft_putchar_fd.c\
ft_putstr_fd.c\
ft_putendl_fd.c\
ft_putnbr_fd.c\
ft_itoa.c\
ft_strtrim.c\
ft_split.c


DEPS = libft.h
OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "Building $(NAME)"

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re