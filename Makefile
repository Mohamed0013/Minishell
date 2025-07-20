# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -g3 #-Werror
LDFLAGS = -lreadline -Llibft -lft #-fsanitize=address -g3
#-fsanitize=address
# Directories
LIBFT_DIR = libft
INCLUDES = -Iincludes -I$(LIBFT_DIR)/includes

# Source files (removed libft functions)
SRC = parsing/parsing.c\
	  tokens/tokenizer.c\
	  tokens/test_tokenizer.c\
	  expansion/expansion.c\
	  expansion/test_expansion.c\
	  main.c\
	  main_utils.c\
	  utils/env.c\
	  utils/empty_line.c\
	  execution/shell.c\
	  execution/builtins/cd.c\
	  execution/builtins/utils.c\
	  execution/utils.c\
	  execution/builtins/echo.c\
	  execution/builtins/env.c\
	  execution/builtins/export.c\
	  execution/utils2.c\
	  execution/builtins/pwd.c\

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
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

# Compiling object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

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
