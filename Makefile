NAME    = minishell

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g -Iincludes -I$(LIBFT_DIR)

SRC_DIR = srcs
OBJ_DIR = objs
INC_DIR = includes
LIBFT_DIR = libft

READLINE = -lreadline

SRCS    = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/execution/executor.c \
	$(SRC_DIR)/execution/init_env.c \
	$(SRC_DIR)/execution/pipes.c \
	$(SRC_DIR)/execution/redirections.c \
	$(SRC_DIR)/execution/builtin/ft_cd.c \
	$(SRC_DIR)/execution/builtin/ft_echo.c \
	$(SRC_DIR)/execution/builtin/ft_env.c \
	$(SRC_DIR)/execution/builtin/ft_exit.c \
	$(SRC_DIR)/execution/builtin/ft_pwd.c \
	$(SRC_DIR)/execution/builtin/ft_export.c \
	$(SRC_DIR)/execution/builtin/ft_unset.c \
	$(SRC_DIR)/execution/utils/free.c \
	$(SRC_DIR)/execution/utils/utils00.c \
	$(SRC_DIR)/execution/utils/env_utils.c \
	$(SRC_DIR)/parsing-part/tokenizer.c \
	$(SRC_DIR)/parsing-part/process_input.c \
	$(SRC_DIR)/parsing-part/print_error.c \
	$(SRC_DIR)/parsing-part/parse_tokens.c \
	$(SRC_DIR)/parsing-part/is_only_spaces.c \
	$(SRC_DIR)/parsing-part/heredoc.c \
	$(SRC_DIR)/parsing-part/heredoc_utils.c \
	$(SRC_DIR)/parsing-part/get_token_type.c \
	$(SRC_DIR)/parsing-part/ft_strjoin_char.c \
	$(SRC_DIR)/parsing-part/ft_realloc_array.c \
	$(SRC_DIR)/parsing-part/expand_variables_and_remove_quotes.c \
	$(SRC_DIR)/parsing-part/check_cmds.c \
	$(SRC_DIR)/parsing-part/cases_of_expanding.c \
	$(SRC_DIR)/parsing-part/cases_of_expanding_plus.c

OBJS    = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(NAME): $(LIBFT_DIR)/libft.a $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(READLINE) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	make -C $(LIBFT_DIR)

all: $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re