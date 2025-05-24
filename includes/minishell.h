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
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../libft/libft.h"

// Define the maximum length for command input
#define MAX_CMD_LENGTH 1024

// Structure for linked list nodes to store commands
typedef struct s_command {
    char *full_command;  // Full command string
    char *command;       // Command name (e.g., "ls")
    char **arguments;    // Command arguments
    char *input_file;    // For '<' (not implemented yet)
    char *output_file;   // For '>' or '>>'
    int append;          // 1 if '>>', 0 otherwise
    struct s_command *next; // Pointer to the next command (for pipelines)
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

//special commands
void execute_echo(char **arguments);
void execute_cd(char **arguments);

//pipes
void handle_pipes(char *input, char **env);
char **split_commands(char *input);

//execution
char	*get_path(char *cmd, char **env);
void    execute_command(t_command *cmd, char **env);

//parsing
void parse_and_execute(char *input, char **env);
// Function to parse a single command from input
t_command *parse_command(char *input);
void free_commands(t_command *head);
void  free_command(t_command *cmd);

//redirections
void handle_append_redirection(char *command, char **env);
void handle_output_redirection(char *command, char **env);


//hado 39l 3lihom
void execute_single_command(t_command *cmd, char **env);
void    free_split(char **split);
int     get_len(char **s);
int is_builtin(t_command *cmd);
void execute_builtin(t_command *cmd);

#endif // MINISHELL_H