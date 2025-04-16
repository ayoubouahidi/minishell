NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -I/home/elkharti/projects/minishell/includes
LDFLAGS = -lreadline

SRCS = srcs/main.c srcs/parsing/parser.c srcs/execution/builtin.c \
	srcs/execution/utils/utils00.c srcs/execution/executor.c

OBJ = $(SRCS:.c=.o)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
