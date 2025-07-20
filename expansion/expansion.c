// #include "expansion.h"
#include "../includes/minishell2.h"
#include "../includes/minishell.h"
#include "../includes/parsing.h"
#include "../includes/expansion.h"

char *get_env_value(t_env *env,  char *var_name)
{
    if (!env || !var_name || !*var_name)
        return NULL; // Return NULL if env or var_name is NULL or empty
    while (env)
    {
        // if (!env->name || !var_name)
        //     continue ;

        if (ft_strlen(env->name) == ft_strlen(var_name) && ft_strncmp(env->name, var_name, ft_strlen(var_name)) == 0)
        {
            return (env->value); // Return the value of the variable
        }
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

// // Helper: get variable value from env (returns "" if not found)
// static const char *get_env_val(t_env *env, const char *name) {
//     while (env) {
//         if (strcmp(env->name, name) == 0)
//         {
//             // return ft_strchr(env->value, '=') + 1;
//             return env->value; // Return the value of the variable
//         }
//         env = env->next;
//     }
//     return "";
// }

bool prev_not_heredoc(t_token *token) {
    if (!token || !token->prev)
        return true; // No previous token, assume not heredoc
    return token->prev->type != TOKEN_HEREDOC; // Check if the previous token is not a heredoc
}

bool prev_not_redirect(t_token *token) {
    if (!token || !token->prev)
        return true; // No previous token, assume not redirect
    return !ft_token_is_redirection(token->prev->type); // Check if the previous token is not a redirection
}

// Helper: expand a single argument string
static char *expand_arg(const char *arg, t_env *env, t_token *token) {
    if (!arg || !*arg || !env)
        return NULL; // Return NULL if arg is empty or env is NULL
    size_t len = ft_strlen(arg);
    char *result = malloc(len * 4 + 1); // Generous allocation
    if (!result) return NULL;

    size_t i = 0, j = 0;
    int in_squote = 0, in_dquote = 0;

    while (arg[i]) {
        // Handle single quotes
        if (arg[i] == '\'' && !in_dquote) {
            in_squote = !in_squote;
            i++;
            continue;
        }

        // Handle double quotes
        if (arg[i] == '"' && !in_squote) {
            in_dquote = !in_dquote;
            i++;
            continue;
        }

        // Handle variable expansion
        if (arg[i] == '$' && !in_squote && prev_not_redirect(token) 
            && (ft_isalpha(arg[i + 1]) || arg[i + 1] == '_')) {
            size_t var_start = i + 1;
            size_t var_len = 0;

            // Extract variable name
            while (ft_isalnum(arg[var_start + var_len]) || arg[var_start + var_len] == '_') {
                var_len++;
            }

            char var_name[128];
            ft_strlcpy(var_name, arg + var_start, var_len + 1);

            // Get variable value
            const char *val = get_env_value(env, var_name);
            size_t vlen = ft_strlen(val);

            // Copy variable value to result
            ft_memcpy(result + j, val, vlen);
            j += vlen;
            i = var_start + var_len;
            continue;
        }

        // Copy regular characters
        result[j++] = arg[i++];
    }

    result[j] = '\0'; // Null-terminate the result
    return result;
}

void expand(t_env *env, t_token *token)
{
    if (!env || !token)
        return;
    t_token *current = token;
    while (current && current->type != TOKEN_EOF)
    {
        if (current->type == TOKEN_WORD)
        {
            // printf("Expanding token: %s\n", current->value); // Debug output
            char *expanded = expand_arg(current->value, env, current);
            if (expanded)
            {
                free(current->value); // Free the old value
                current->value = expanded; // Assign the new expanded value
                // printf("Expanded token: %s\n", current->value); // Debug output
            }
        }
        current = current->next; // Move to the next token
    }
        
    // t_list *args = ast->args;
    // while (args)
    // {
    //     t_token *token = NULL;
    //     token->value = (char *)args->content; // Assuming args->content is a string
    //     printf("Expanding token: %s\n", (char *)args->content);
    //     char *arg = token->value;
    //     // if (token->type == TOKEN_WORD && !arg && !token->value)
    //     // {
    //     //     args = args->next;
    //     //     continue; // Skip empty tokens
    //     // }
    //     if (!arg) {
    //         args = args->next;
    //         continue;
    //     }
    //     char *expanded = expand_arg(arg, env);
    //     if (expanded) {
    //         free(token->value);
    //         token->value = expanded;
    //     }
    //     args = args->next;
    // }
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