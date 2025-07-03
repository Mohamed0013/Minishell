// #include "expansion.h"
#include "../includes/minishell2.h"
#include "../includes/minishell.h"
#include "../includes/parsing.h"

char *get_env_value(t_env *env, const char *var_name)
{
    while (env)
    {
        if (strcmp(env->name, var_name) == 0)
            return ft_strchr(env->value,'=') + 1; // Return the value of the variable
        env = env->next;
    }
    return NULL; // Variable not found
}

int mask_len(t_token *token)
{
    if (!token || !token->value)
        return 0;

    int len = 0;
    int i = 0;
    char c;
    char *value = token->value;
    while (*value)
    {
        if (*value == '\'' || *value == '"')
        {
            c = *value; // Store the quote character
            value++; // Skip the quote
            while (*value && *value != c)
            {
                len++;
                value++;
            }
            if (*value == c) // Skip the closing quote
                value++;
        }
        else if (*value == '$' && (isalpha(*(value + 1)) || *(value + 1) == '_'))
        {
            value++; // Skip the '$'
            i = 0;
            while (isalnum(value[i]) || value[i] == '_')
            {
                i++;
            }
            c = value[i];
            value[i] = '\0'; // Temporarily null-terminate the string
            char *env_value = getenv(value); // Get the environment variable
            len += strlen(env_value); // Get the length of the environment variable
            value[i] =c;
            value+= i;
        }
        else
        {
            len++; // Count the character
            value++;
        }
    }
    return len;
}

void    expand(t_ast *ast, t_env *env)
{
    if (!ast || !env)
        return;

    t_list *args = ast->args;
    char    *mask;
    int     len = mask_len((t_token *)args->content); // Get the length of the mask
    mask = malloc(len + 1); // Allocate memory for the mask
    if (!mask)
    {
        ft_putstr_fd("Memory allocation failed for mask.\n", 2);
        return;
    }
    
    while (args)
    {
        // char *arg = (char *)args->content;
        t_token *token = (t_token *)args->content;
        char *arg = token->value; // Get the value of the token
        if (!arg)
        {
            args = args->next;
            continue; // Skip if the argument is NULL
        }
        // Check if the argument starts with '$' for variable expansion
        if (arg && arg[0] == '$') // Check for variable expansion
        {
            char *var_name = arg + 1; // Skip the '$'
            char *value = get_env_value(env, var_name);
            if (value)
            {
                free(arg);
                args->content = strdup(value); // Replace with the value from env
            }
        }
        args = args->next;
    }
}






























/*
void    expand



                IS IN DOUBLE QUOTE
                v
             00011
                 ^
                 EXPANDED OR NOT
0000  ORIGINAL
0001  EXPANDED
0010  ORIGINAL IN DOUBLE QUOTE
0011  EXPANDED IN DOUBLE QUOTE
0100  ORIGINAL IN SINGLE QUOTE


x="a b"

lsa b"a b"'$x' 
00111033300440
*/

// a="ls  -l"


// {
//     int  type ;
//     char * value ;
//     t_list fields;
// }

// t_ast {
//     t_list args;
//     t_list redirect;
//     next; 
// }


// $a  


// {
//     type = TOKEN_WORD;
//     value = "$a"
//     fields = ls -> -l

// }


// [ls,-l]