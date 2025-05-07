CC = cc
LDFLAGS = -lreadline
# CFLAGS = -Wall -Wextra -Werror
# Source files
SRC = src/main.c \
      src/syntax/valid_syntax.c \
	  src/cmd.c\
	  src/integrate.c\
	  src/libft_functs1.c\
	  src/libft_utils.c\
	  src/parse_it.c\
	  src/utils.c\

# Object files
OBJ = $(SRC:.c=.o)

# Output binary name
NAME = minishell

# Default target
all: $(NAME)

# Linking the binary
$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

# Compiling object files
%.o: %.c
	$(CC) -c $< -o $@

# Cleaning object files
clean:
	rm -f $(OBJ)

# Cleaning everything
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re