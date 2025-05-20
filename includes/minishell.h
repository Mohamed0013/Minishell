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
#include "../libft/libft.h"

// Define the maximum length for command input
#define MAX_CMD_LENGTH 1024
//
typedef struct s_env
{
    char *name;
    char *value;
    struct s_env *next;
} t_env;
//
typedef struct s_data
{
    t_env *env_list;
    int exit_status;
} t_data;

// Declare your global variable:
extern t_data g_data;

typedef struct s_global
{
    t_env *env_list; // Environment variables
    int exit_status; // Last exit status
} t_global;
// Structure for linked list nodes to store commands
typedef struct s_command
{
    char *command;
    char **arguments;
    char *full_command;
    struct s_command *next;
} t_command;
//
t_env *lst_new_env(char *name, char *value);
void lst_add_env(t_env **lst, t_env *new);
//
/* Command parsing and execution */
void multi_to_single_space(char **av, char *res, int ac);
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

// builtins
// int ft_env(t_env *env_list);
int ft_strcmp(const char *s1, const char *s2);
int is_valid_env_name(const char *name);
void update_or_add_env(t_env **env, char *name, char *value);
int validate_and_split(char *arg, char **name, char **value);
t_env *env_from_array(char **env);
int ft_export(t_env **env, char **args);
int ft_env(t_env *env_list, char **args);
int ft_unset(t_env **env, char **args);

void execute(t_command *cmd, char **env);

// special commands
void execute_echo(char **arguments);
void execute_cd(char **arguments);

// pipes
void handle_pipes(t_command *cmd, char **env);

void free_split(char **split);
int get_len(char **s);

// execution
char *get_path(char *cmd, char **env);
void execute_command(t_command *cmd, char **env);

#endif // MINISHELL_H
