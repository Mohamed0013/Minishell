#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_CMD_LENGTH 1024
# define HEREDOC_MSG "minishell: heredoc: EOF reached withoutdelimiter\n"
# define HEREDOC_TMP_FILE "/tmp/heredoc.tmp"

typedef struct execute_s
{
	int					exit_status;
	int					**pipfds;
	int					nb_pipes;
}						t_execute;

typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_data
{
	t_env				*env_list;
	int					exit_status;
}						t_data;

extern t_data			g_data;

typedef struct s_global
{
	t_env				*env_list;
	int					exit_status;
}						t_global;

typedef struct s_command
{
	char				**arguments;
	char				*full_command;
	struct s_command	*next;
}						t_command;

typedef enum e_token_type
{
	TOKEN_WORD = 0,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	int					is_quoted;
	struct s_token		*prev;
	struct s_token		*next;
}						t_token;

typedef struct s_ddate
{
	t_token				*head;
	const char			*ptr;
	size_t				len;
}						t_ddata;

typedef struct s_child_data
{
	char				**full_command;
	char				*path;
	int					status;
}						t_child_data;

typedef struct s_exec_data
{
	t_execute			*exec;
	int					nb_pipes;
	int					ret;
	char				**full_command;
	int					exit_status;
}						t_exec_data;

typedef struct s_external_data
{
	pid_t				pid;
	int					status;
	char				*path;
}						t_external_data;

typedef struct s_redir
{
	t_token_type		type;
	char				*filename;
	char				*value;
}						t_redir;

typedef struct s_ast
{
	t_list				*args;
	t_list				*redirections;
	struct s_ast		*next;
}						t_ast;

typedef struct s_pipe_data
{
	t_ast				*current;
	int					i;
	pid_t				*pids;
	int					exit_check;
}						t_pipe_data;

void					expand(t_env *env, t_token *token);
char					*get_env_value(t_env *env, char *var_name);
int						mask_len(t_token *token);
void					free_env(t_env *env);
void					print_args(t_token *args);
void					test_expansion(const char *input, t_list *env);

t_env					*lst_new_env(char *name, char *value);

/* Command parsing and execution */
void					multi_to_single_space(char **av, char *res, int ac);
t_command				*create_command(char *cmd);
void					print_command(t_command *cmd);
void					print_command_list(t_command *cmd);
t_command				*parse_input(char *input, char **env);
void					add_command(t_command **head, t_command *new_cmd);
void					shell_loop(t_command *cmd, char **env);

/* Syntax validation */
bool					is_blank_line(const char *s);

/* Minishell-specific utils */
char					*join_path(char *path, char *bin);
char					*str_ndup(char *str, unsigned int n);
int						str_ichr(const char *str, char c);

int						execute_pwd(void);

int						ft_strcmp(char *s1, char *s2);
int						is_valid_env_name(const char *name);
void					update_or_add_env(t_env **env, char *name, char *value);
int						validate_and_split(char *arg, char **name,
							char **value);
t_env					*env_from_array(char **env);
char					**env_to_array(t_env *env_list);
int						ft_export(t_env **env, char **args);
int						ft_env(t_env *env_list, char **args);
int						ft_unset(t_env **env, char **args);

void					execute(t_command *cmd, char **env);

void					execute_echo(char **arguments, int status);

void					handle_append_redirection(char *command, char **env);
void					handle_output_redirection(char *command, char **env);
void					handle_input_redirection(char *command, char **env);

void					free_split(char **split);
int						get_len(char **s);

char					*get_path(char *cmd, char **env);

t_command				*parse_command(char *command);
int						ft_split_size(char **split);
int						get_len(char **s);

/* Quote handling */

void					free_env_list(t_env *env);

bool					is_blank_line(const char *s);
t_env					*env_from_array(char **env);
char					**env_to_array(t_env *env_list);
char					*str_ndup(char *str, unsigned int n);
char					*ft_strchr(const char *s, int c);

void					initial_signals(void);
void					handle_sigint(int sig);
void					handle_sigquit(int sig);
int						free_input(char *input);
int						free_env_and_input(char *input, int value);

t_token					*create_token(char *value, t_token_type type,
							int in_quote);
void					add_token(t_token **head, t_token *new_token);
void					free_tokens(t_token *head);
void					print_tokens(t_token *head);
t_token					*tokenize_input(const char *input);
t_token					*tokenize(const char *input);

t_ast					*parser(const char *input);
t_token					*tokenize(const char *input);
void					free_ast(t_ast *ast);
void					free_ast_list(t_ast *head);
void					add_ast_node(t_ast **head, t_ast *new_node);
bool					ft_token_is_redirection(t_token_type type);
void					print_ast(t_ast *ast);
int						ft_lst_push(t_list **head, void *value);
void					add_token(t_token **head, t_token *new_token);
void					free_tokens(t_token *head);
t_token					*tokenize_input(const char *input);
void					print_tokens(t_token *tokens);
void					free_commands(t_ast *cmd);

int						shell_execute(t_ast *ast, char **env, int status);

void					free_split(char **split);
void					free_commands(t_ast *cmd);
int						str_ichr(const char *str, char c);
char					*join_path(char *path, char *bin);

void					execute_cd(char **arguments);
int						ft_env(t_env *env_list, char **args);
int						ft_export(t_env **env, char **args);

int						get_len(char **s);

int						handle_pipes(t_ast *ast, int nb_pipes, t_execute *exec,
							char **env);

int						execute_single_command(t_exec_data *data, t_ast *ast,
							char **env);
void					execute_child_external_pipe(t_child_data *data,
							char **env);
void					execute_child_builtin(t_child_data *data,
							t_ast *current, t_execute *exec);
void					close_parent_pipes(t_execute *exec, int i,
							int nb_pipes);
void					setup_child_pipes(t_execute *exec, int i, int nb_pipes,
							t_list *redir);
int						set_pipefds(int nb_pipes, t_execute *exec);
void					fill_pipes(int **pipfds, int nb_pipes);
int						check_redirect_types(t_list *redirections,
							int *has_output, int *has_input);
void					cleanup_pipes_on_error(int **pipfds, int i);
int						count_pipes(t_ast *ast);
char					*ft_strconcat(char **args, const char *sep);
char					**get_args(t_list *args);
char					*dup_and_check(t_list *current, char **result, int *i);
int						execute_command(t_execute *exec, char **cmd,
							t_list *redir, char **env);
void					exec_error(int err, char **cmd);
int						execute_builtin_direct(t_execute *exec, char **cmd,
							t_list *redir);
void					restore_fds(t_list *redirections, int *saved_fds);
int						save_restore_fds(t_list *redirections, int *saved_fds);
int						execute_builtin_piped(t_execute *exec, char **cmd,
							t_list *redir);
int						handle_redirections(t_list *redirections);
int						execute_builtin(char **args, t_list *redirections,
							int exit_status);
int						exec_builtin_part1(char **args, int exit_status);
int						exec_builtin_part2(char **args, t_list *redirections);
void					free_exec(t_execute *exec);
int						is_builtin(char *command);

void					exec_utils(t_exec_data *data, t_ast *ast, int status);
int						exec_utils2(t_exec_data *data, t_ast *ast,
							char **current_env, char **env);

void					fill_arr(t_env **arr, t_env *temp, t_env *swap,
							int count);
void					check_and_free(char *value, char *name);
void					print_util(int i, int count, t_env **arr);

int						ft_exit_withclear(int code);
bool					ft_token_is_redirection(t_token_type type);
t_ast					*create_ast_node(void);
void					add_ast_node(t_ast **head, t_ast *new_node);
bool					ft_token_is_redirection(t_token_type type);
int						ft_lst_push(t_list **head, void *value);
void					free_ast(t_ast *ast);

char					*handle_heredoc(char *delimiter);

t_list					**gc_ptr(void);
void					*ft_malloc(size_t size);
void					ft_gc_clear(void);
void					ft_gc_add(void *ptr);

#endif
