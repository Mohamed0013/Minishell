#include "../includes/minishell2.h"
#include "../includes/minishell.h"
#include "../includes/parsing.h"

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
// void free_ast_list(t_ast *head)
// {
//     t_ast *current = head;
//     while (current)
//     {
//         t_ast *next = current->next;
//         free_ast(current);
//         current = next;
//     }
// }
// void print_ast(t_ast *ast)
// {
//     if (!ast)
//         return;
//     printf("AST Node:\n");
//     printf("Arguments: ");
//     // print_list(ast->args);
//     printf("Redirections: ");
//     // print_list(ast->redirections);
//     if (ast->next)
//         printf("Next node exists.\n");
//     else
//         printf("No next node.\n");
// }

bool ft_token_is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND 
            || type == TOKEN_HEREDOC);
}

void    ft_lst_push(t_list **head, void *value)
{
    t_list *new_node = ft_lstnew(value);
    if (!new_node)
        return;
    ft_lstadd_back(head, new_node);
}

void free_ast(t_ast *ast)
{
    if (!ast)
        return;

    // Free linked arguments list
    if (ast->args)
        ft_lstclear(&ast->args, free);

    // Free linked redirections list 
    if (ast->redirections)
        ft_lstclear(&ast->redirections, free);

    // Recursively free next nodes
    if (ast->next)
        free_ast(ast->next);

    free(ast);
}

t_ast *parser(const char *input)
{
    t_token *tokens = tokenize(input);
    t_token *current = tokens;
    if (!tokens)
    {
        ft_putstr_fd("Error tokenizing input.\n", 2);
        return NULL;
    }
    while (tokens && tokens->type != TOKEN_EOF)
    {
        // t_token *temp = tokens;
        printf("- %-20s\t %s\n", ft_token_gettype(tokens->type), tokens->value);
        tokens = tokens->next;
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
            ft_lst_push(&curr->args, current->value);
        else if (ft_token_is_redirection(current->type))
        {
            if (current->next && current->next->type == TOKEN_WORD)
            {
                // t_redir *redir = malloc(sizeof(t_redir));
                // if (!redir)
                //     return NULL; // Handle memory allocation failure
                // redir->type = current->type; // Set the type of redirection
                // Set the filename for the redirection
                current->value = current->next->value; // Get the filename from the next token
                // char *redirection = current->next->value;
                ft_lst_push(&curr->redirections, current);
                current = current->next; // Skip the next word as it's already processed
            }
            else
            {
                ft_lst_push(&curr->redirections, current->value);
                printf("Redirection without target%s\n", current->value);
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
