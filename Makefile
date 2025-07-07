NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

READLINE = -lreadline
INCLUDES = -Iincludes -I$(LIBFT_DIR)

SRCS = \
	srcs/main.c \
	srcs/execution/executor.c \
	srcs/execution/init_env.c \
	srcs/execution/pipes.c \
	srcs/execution/redirections.c \
	srcs/execution/signals.c \
	srcs/execution/utils/ft_malloc.c \
	srcs/execution/utils/builtin_utils.c \
	srcs/execution/utils/utils00.c \
	srcs/execution/utils/utils01.c \
	srcs/execution/utils/env_utils.c \
	srcs/execution/builtin/ft_cd.c \
	srcs/execution/builtin/ft_echo.c \
	srcs/execution/builtin/ft_env.c \
	srcs/execution/builtin/ft_exit.c \
	srcs/execution/builtin/ft_pwd.c \
	srcs/execution/builtin/ft_export.c \
	srcs/execution/builtin/ft_unset.c \
	srcs/parcing/parcer.c \
	srcs/parcing/parcer_utils_1.c \
	srcs/parcing/parcer_utils_2.c \
	srcs/parcing/parcer_utils_3.c \
	srcs/parcing/parcer_utils_4.c \
	srcs/parcing/parcer_utils_5.c \
	srcs/parcing/parcer_utils_6.c \
	srcs/parcing/heredoc.c \
	srcs/parcing/heredoc_utils.c \
	srcs/parcing/expantion.c \
	srcs/parcing/expantion_utils_1.c \
	srcs/parcing/expantion_utils_2.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBFT) $(READLINE)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) bonus

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: ${OBJS}