# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -g3 #-Werror
LDFLAGS = -lreadline -Llibft -lft #-fsanitize=address -g3
#-fsanitize=address
# Directories
LIBFT_DIR = libft
INCLUDES = -Iincludes -I$(LIBFT_DIR)/includes

# Source files (removed libft functions)
SRC = ./execution/builtins/cd.c \
./execution/builtins/echo.c \
./execution/builtins/env.c \
./execution/builtins/exit.c \
./execution/builtins/export.c \
./execution/builtins/pwd.c \
./execution/builtins/utils/utils1.c \
./execution/builtins/utils/utils2.c \
./execution/builtins/utils/utils3.c \
./execution/exec_utils.c \
./execution/exit_clear.c \
./execution/redir_utils/utils.c \
./execution/redir_utils/utils2.c \
./execution/shell.c \
./execution/shell_utils/pipeutils.c \
./execution/shell_utils/pipeutils2.c \
./execution/shell_utils/pipeutils3.c \
./execution/shell_utils/pipeutils4.c \
./execution/shell_utils/utils1.c \
./execution/shell_utils/utils2.c \
./execution/utils.c \
./execution/utils2.c \
./execution/utils3.c \
./expansion/expansion.c \
./expansion/utils/check_utils.c \
./expansion/utils/expand_args.c \
./expansion/utils/get_env_value.c \
./expansion/utils/get_masklen.c \
./expansion/utils/handle_qotes.c \
./expansion/utils/handle_vars.c \
./expansion/utils/init_context.c \
./expansion/utils/process_env_var.c \
./expansion/utils/quoted_str.c \
./expansion/utils/split_expansion.c \
./garbage_collector/ft_malloc.c \
./main.c \
./main_utils.c \
./parsing/help_parsing.c \
./parsing/parsing.c \
./parsing/help_parsing2.c \
./tokens/token_utils.c \
./tokens/token_utils2.c \
./tokens/token_utils3.c \
./tokens/tokenizer.c \
./utils/empty_line.c \
./utils/env.c


# Object files
OBJ = $(SRC:.c=.o)

# Output binary name
NAME = minishell

# Default target
all: libft $(NAME)

# Build libft first
libft:
	@$(MAKE) -C $(LIBFT_DIR) bonus

# Linking the binary
$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(INCLUDES) $(LDFLAGS) 

# Compiling object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@  $(INCLUDES)

# Cleaning object files
clean:
	rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean

# Cleaning everything
fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re libft

