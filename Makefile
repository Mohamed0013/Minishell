# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline -Llibft -lft -fsanitize=address

# Directories
LIBFT_DIR = libft
INCLUDES = -Iincludes -I$(LIBFT_DIR)/includes

# Source files (removed libft functions)
SRC = src/main.c \
      src/cmd.c \
      src/integrate.c \
      src/parse_it.c \
      src/utils.c \
	  src/libft_utils.c\
      src/syntax/validate_syntax.c \
      src/syntax/files_syntax.c \
      src/syntax/pipe_syntax.c \
      src/syntax/syntax.c\
	  src/special_commands/execute_cd.c\
	  src/special_commands/execute_echo.c\
	  src/pipes/handle_pipes.c\
	  builtins/env.c  builtins/export.c  builtins/export_utils.c \
	  src/executor.c

# Object files
OBJ = $(SRC:.c=.o)

# Output binary name
NAME = minishell

# Default target
all: libft $(NAME)

# Build libft first
libft:
	@$(MAKE) -C $(LIBFT_DIR)

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