#include "../includes/minishell2.h"
#include "../includes/minishell.h"
#include "../includes/parsing.h"
#include "../includes/expansion.h"

// char * ft_token_gettype(t_token_type type);
t_ast *create_ast_node(void)
{
    t_ast *node = malloc(sizeof(t_ast));
    if (!node)
        return NULL;
    node->args = NULL;
    node->redirections = NULL;
    node->next = NULL;
    return node;
}

void add_ast_node(t_ast **head, t_ast *new_node)
{
    if (!head || !new_node)
        return;
    if (!*head)
    {
        *head = new_node;
    }
    else
    {
        t_ast *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

bool ft_token_is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND 
            || type == TOKEN_HEREDOC);
}

int    ft_lst_push(t_list **head, void *value)
{
    t_list *new_node = ft_lstnew(value);
    if (!new_node)
        return (0);
    ft_lstadd_back(head, new_node);
    return (1);
}

void free_ast(t_ast *ast)
{
    if (!ast)
        return;

    // Free linked arguments list
    if (ast->args)
        ft_lstclear(&ast->args, free);

    // Free linked redirections list 
    if (ast->redirections) {
        t_list *curr = ast->redirections;
        while (curr) {
            t_redir *redir = (t_redir *)curr->content;
            if (redir) {
                if (redir->filename)
                    free(redir->filename);
                free(redir);
            }
            curr = curr->next;
        }
        ft_lstclear(&ast->redirections, NULL); // Don't double free content
    }

    // Recursively free next nodes
    if (ast->next)
        free_ast(ast->next);

    free(ast);
}

t_ast *parser(const char *input)
{
    t_token *tokens = tokenize(input);
    t_token *current = tokens;
    t_token *print_token;
    if (!tokens)
    {
        ft_putstr_fd("Error tokenizing input.\n", 2);
        return NULL;
    }
    expand(g_data.env_list, tokens);
    print_token = tokens;
    while (print_token && print_token->type != TOKEN_EOF)
    {
        // t_token *temp = print_token;
        printf("- %-20s\t %s\n", ft_token_gettype(print_token->type), print_token->value);
        print_token = print_token->next;
    }
    t_ast *ast = NULL;
    t_ast *curr = create_ast_node();
    ast = curr; // Initialize the AST with the first node
    if (!curr)
    {
        ft_putstr_fd("Error creating AST node.\n", 2);
        free_tokens(tokens);
        if (ast)
            free_ast(ast);
        return NULL;
    }
    while (current)
    {     
        if (current->type == TOKEN_EOF)
            break;
        else if (current->type == TOKEN_WORD)
        {
            char *arg_copy = ft_strdup(current->value);
            if (!arg_copy)
            {
                if (ast)
                    free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
            if (!ft_lst_push(&curr->args, arg_copy))
            {
                free(arg_copy);
                if (ast)
                    free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
        }
        else if (ft_token_is_redirection(current->type))
        {
            if (current->next && current->next->type == TOKEN_WORD)
            {
                t_redir *redir = malloc(sizeof(t_redir));
                if (!redir)
                {
                    if (ast)
                        free_ast(ast);
                    free_tokens(tokens);
                    return NULL;
                }
                redir->type = current->type;
                redir->filename = ft_strdup(current->next->value); // For heredoc, this is the delimiter
                if (!redir->filename)
                {
                    free(redir);
                    if (ast)
                        free_ast(ast);
                    free_tokens(tokens);
                    return NULL;
                }
                if (!ft_lst_push(&curr->redirections, redir))
                {
                    free(redir->filename);
                    free(redir);
                    if (ast)
                        free_ast(ast);
                    free_tokens(tokens);
                    return NULL;
                }
                current = current->next; // Skip the next word as it's already processed
            }
            else
            {
                // Error: redirection without target
                if (ast)
                    free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
        }
        else if (current->type == TOKEN_PIPE)
        {
            // next pipe should be a word or a redirection
            if (current->next->type != TOKEN_WORD && !ft_token_is_redirection(current->next->type))
            {
                ft_putstr_fd("Syntax error: Pipe not followed by a command or redirection.\n", 2);
                if (ast)
                    free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
            t_ast *new_node = create_ast_node();
            if (!new_node)
            {
                ft_putstr_fd("Error creating new AST node for pipe.\n", 2);
                if (ast)
                    free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
            add_ast_node(&ast, new_node);
            curr = new_node; // Move to the new node
        }
 
        else
        {
            // fprintf(stderr, "Unexpected token type: %d\n", current->type);
            ft_putstr_fd("Unexpected token type: ", 2);
            ft_putnbr_fd(current->type, 2);
            ft_putstr_fd("\n", 2);
            if (ast)
                free_ast(ast);
            free_tokens(tokens);
            return NULL;
        }
        current = current->next;
    }
    free_tokens(tokens);
    return (ast);
}
