#include "../includes/minishell.h"
#include "../includes/minishell2.h"
#include "../includes/parsing.h"
#include "../includes/expansion.h"

// Dummy env list for testing
int ft_keycmp(char * keyvalue,char* key)
{
    size_t key_len;
    if (keyvalue == NULL || key == NULL)
        return 0;
    key_len = ft_strlen(key);
    return (strncmp(keyvalue, key,key_len) == 0 && keyvalue[key_len] == '=');

}
int ft_keyvalcmp(char * k1,char* k2)
{
    size_t key_len;

    if(k1 == k2)
        return 1;
    if (k1 == NULL || k2 == NULL)
        return 0;
    key_len = 0; 
    while(1)
    {
        if(k1[key_len] != k2[key_len])
            return 0;
        if(k1[key_len] == '=' || k1[key_len] == '\0')
            return 1;
        key_len++;
    }
   return 1;
}

void env_add(t_list **env, char *value)
{
    t_list *curr_p;
    t_list *prev_p;
    // *env_list = NULL;
    if(env == NULL || value == NULL || value[0] == '\0')
        return ;
    // remove existing keys if exit in the list (the value is key=value)
    prev_p = NULL;
    curr_p = *env;
    while(*env &&  ft_keycmp((*env)->content, value))
    {
        curr_p = (*env)->next;
        ft_lstdelone(*env, free);
        *env = curr_p;
    }
    while (curr_p)
    {
        
        if (ft_keyvalcmp(curr_p->content, value))
        {
            prev_p->next = curr_p->next; // Remove the current node from the list
            ft_lstdelone(curr_p, free); // Free the old node
            curr_p = prev_p; // Move to the next node
        }
        prev_p = curr_p;
        curr_p = curr_p->next;
    }
    ft_lstadd_back(env, ft_lstnew(ft_strdup(value)));
    // return (t_env *)env; // Cast to t_env for compatibility with the rest
    // t_env *env = malloc(sizeof(t_env));
    // env->name = strdup(name);
    // env->value = strdup(value);
    // env->next = NULL;
    // return env;
}

void free_env(t_env *env)
{
    while (env)
    {
        t_env *next = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = next;
    }
}

void print_args(t_token *token)
{
    int i = 0;

    t_token *current = token;
    while (current && current->type != TOKEN_EOF)
    {
        printf("-----Arg[%03d]: %s \n", i++, current->value);
        current = current->next; // Move to the next token
    }
}

void test_expansion(const char *input, t_list *env)
{
    printf("\nInput: %s\n", input);
    t_token *tokens = tokenize(input);
    if (!tokens)
    {
        printf("Tokenization failed!\n");
        return;
    }
    t_ast *ast = parser(input);
    if (!ast)
    {
        printf("Parsing failed!\n");
        free_tokens(tokens);
        return;
    }
    print_args(tokens);
    // expand(ast, env, tokens);
    printf("After expansion:\n");
    print_args(tokens);
    // free_ast(ast);
    free_tokens(tokens);
}

// int main(int argc,char ** argv,char **envp)
// {
//     (void)argc; // Unused parameter
//     (void)argv; // Unused parameter 
//     t_list *env = NULL; // Initialize the environment list
//     // Setup environment: HOME=/home/user, USER=test, PATH=/bin:/usr/bin
//     while(*envp)
//     {
//         env_add(&env, *envp); // This is a dummy function to create env variables
//         envp++;
//     }
//     env_add(&env, "a=\""); // This is a dummy function to create env variables



//     // t_env *env = make_env("HOME", "/home/user");
//     // env->next = make_env("USER", "test");
//     // env->next->next = make_env("PATH", "/bin:/usr/bin");

//     // test_expansion("echo > $HOME", env);
//     // test_expansion("        ", env);
//     // test_expansion("", env);
//     // test_expansion("echo >> $HOME", env);
//     // test_expansion("echo << $HOME", env);
//     // test_expansion("echo > < $HOME", env);
//     // test_expansion("echo $HOME", env);
//     // test_expansion("echo $USER", env);
//     // test_expansion("echo $HOME $USER", env);
//     // test_expansion("echo \"Hello $USER, home: $HOME\"", env);
//     // printf("=======================Testing quotes:\n");
//     // test_expansion("echo '$HOME'", env);
//     // test_expansion("echo \"$USER\"", env);
//     // test_expansion("echo $UNSETVAR", env);
//     // test_expansion("echo $HOME $USER", env);
//     // test_expansion("echo \"Hello $USER, home: $HOME\" | ls | cat file", env);
//     // test_expansion("echo 'Hello $USER, home: $HOME'", env);
//     // test_expansion("echo $PATH", env);
//     test_expansion("echo '$a\"'$PATH'\"$a'", env);

//     //free_env(env);
//     return 0;
// }
