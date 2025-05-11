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

// Define the maximum length for command input
#define MAX_CMD_LENGTH 1024

// Structure for linked list nodes to store commands
typedef struct s_command {
    char *command;
    char **arguments;
    struct s_command *next;
} t_command;

// Function prototypes
void  multi_to_single_space(char **av, char *res, int ac);
char **ft_split(char const *s, char c);
t_command *create_command(char *cmd, char **args);
void free_command(t_command *cmd);
void free_split(char **split);
void free_command_list(t_command *cmd);
void print_command(t_command *cmd);
void print_command_list(t_command *cmd);
void print_command(t_command *cmd);
void print_command_list(t_command *cmd);
void free_commands(t_command *head);
t_command *parse_input(char *input);
void add_command(t_command **head, t_command *new_cmd);
void free_commands(t_command *cmd);
void shell_loop(t_command *cmd, char **env);

#endif // MINISHELL_H