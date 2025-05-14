#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../libft/libft.h"

// typedef enum
// {
//     HEREDOC,
//     REDIR_IN,
//     REDIR_OUT,
//     APPEND,
//     PIPE,
//     CMD,
// } t_type;

// typedef struct s_io 
// {
//     char *file;
//     t_type type;
//     struct s_io *next;
// } t_io;


// typedef struct s_node
// {
//     char *argument; // command to exucute ls -a
//     char *path; // path to the command /bin/ls
//     char **args; // arguments to the command [ls][-a]
//     t_io *io; // input/output redirection

// }t_node;

// Define the maximum length for command input
#define MAX_CMD_LENGTH 1024

// Structure for linked list nodes to store commands
typedef struct s_command {
    char *command;
    char *arguments;
    struct s_command *next;
} t_command;

/* Command parsing and execution */
void  multi_to_single_space(char **av, char *res, int ac);
t_command *create_command(char *cmd);
// void free_command(t_command *cmd);
// void free_command_list(t_command *cmd);
void print_command(t_command *cmd);
void print_command_list(t_command *cmd);
t_command *parse_input(char *input);
void add_command(t_command **head, t_command *new_cmd);
void free_commands(t_command *cmd);
void shell_loop(t_command *cmd, char **env);

/* Syntax validation */
bool check_syntax(const char *input);
bool file_syntax(const char *input);
bool validate_syntax(const char *input);
bool unclosed_quotes(const char *input);
bool is_blank_line(const char *s);
bool pipe_syntax(const char *input);

/* Minishell-specific utils */
char *join_path(char *path, char *bin);
char *str_ndup(char *str, unsigned int n);
int str_ichr(char *str, char c);

void    free_split(char **split);

#endif // MINISHELL_H