#ifndef PARSING_H
#define PARSING_H

#include "minishell2.h"
#include "minishell.h"

typedef struct s_redir
{
    int type; // Type of redirection (e.g., input, output, append)
    char *filename; // Filename for the redirection
}            t_redir;

typedef struct  s_ast
{
    t_list *args; // List of arguments
    t_list *redirections; // List of redirections
    struct s_ast *next; // Pointer to the next command in the pipeline
}               t_ast;

t_ast *parser(const char *input);
t_token *tokenize(const char *input);
void free_ast(t_ast *ast);
void free_ast_list(t_ast *head);
void add_ast_node(t_ast **head, t_ast *new_node);
char *ft_token_gettype(t_token_type type);
bool ft_token_is_redirection(t_token_type type);
void print_ast(t_ast *ast);
void ft_lst_push(t_list **head, void *value);
void add_token(t_token **head, t_token *new_token);
void free_tokens(t_token *head);
t_token *tokenize_input(const char *input);
bool unclosed_quotes(const char *input);
void print_tokens(t_token *tokens);
void free_commands(t_ast *cmd);


//execution
int	    shell_execute(t_ast *ast, char **env, int status);

//execution/utils
int	ft_strcmp(char *s1, char *s2);
void	free_split(char **split);
void	free_commands(t_ast *cmd);
int	str_ichr(const char *str, char c);
char	*join_path(char *path, char *bin);

//builtins
void    execute_cd(char **arguments);
int ft_env(t_env *env_list, char **args);
int ft_export(t_env **env, char **args);


//builtins/utils
int get_len(char **s);

// pipes
int handle_pipes(t_ast *ast, int nb_pipes, t_execute *exec, char **env);

#endif // PARSING_H