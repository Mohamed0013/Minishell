#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

// Define the maximum length for command input
#define MAX_CMD_LENGTH 1024

/* Token types */
// #define WORD    0
// #define PIPE    1
// #define GREAT   2  /* > */
// #define DGREAT  3  /* >> */
// #define LESS    4  /* < */
// #define DLESS   5  /* << */

// Structure for linked list nodes to store commands
// typedef struct s_command
// {
//     char *cmd;
//     char **args;
//     struct s_command *next;
// } t_command;

// Structure for lexer tokens
typedef struct s_lexer
{
    char *value;           // Token value
    // char type;             // Token type (e.g., 'P' for pipe, 'I' for input redirection...)
    // bool in_quotes;        // Whether the token is inside quotes
    // int  token;
    struct s_lexer *next;  // Pointer to the next token
    struct s_lexer *prev;  // Pointer to the previous token
} t_lexer;

// Structure for environment variables
// typedef struct s_env
// {
//     int exit_status; // Stores the exit status of the last command
// } t_env;

// extern t_env *g_env;

// Structure for linked list nodes to store commands
typedef struct s_command {
    char *command;
    char **arguments;
    struct s_command *next;
} t_command;

/* Function prototypes */
t_lexer     *tokenize_input(const char *input);
void        free_lexer(const char **input);
bool        is_blank_line(const char *s);
void        print_syntax_error();
bool        validate_syntax(const char *input);
char        **ft_split(const char *str, char delimiter);

// Syntax validation functions
bool        unclosed_quotes(const char *input);
bool        pipe_syntax(const char *input);
bool        file_syntax(const char *input);
bool        check_syntax(const char *input);

// Function prototypes
char        *ft_strchr(const char *s, int c);
void        multi_to_single_space(char **av, char *res, int ac);
char        **ft_split(char const *s, char c);
t_command   *create_command(char *cmd, char **args);
void        free_command(t_command *cmd);
void        free_split(char **split);
void        free_command_list(t_command *cmd);
void        print_command(t_command *cmd);
void        print_command_list(t_command *cmd);
void        print_command(t_command *cmd);
void        print_command_list(t_command *cmd);
void        free_command(t_command *cmd);
t_command   *parse_input(char *input);
void        add_command(t_command **head, t_command *new_cmd);
void        free_commands(t_command *cmd);
void        shell_loop(t_command *cmd, char **env);

#endif // MINISHELL_H