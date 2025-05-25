#include "../includes/minishell.h"

int ft_env(t_env *env_list, char **args)
{
    t_env *current;

    if (!env_list)
        return (0);

    current = env_list;
    if (args && args[1] != NULL) // Reject arguments
    {
        ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
        return (1);
    }
    while (current)
    {
        if (current->name && current->value) // Only print variables with names and values
            printf("%s=%s\n", current->name, current->value);
        current = current->next;
    }
    return (0);
}

// int ft_env(t_env *env_list, char **args)
// {
//     t_env *current;

//     current = env_list;
//     if (args[1] != NULL) // Reject arguments
//     {
//         ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
//         return (1);
//     }
//     while (current)
//     {
//         if (current->value != NULL) // Only print variables with values
//             printf("%s=%s\n", current->name, current->value);
//         current = current->next;
//     }
//     return (0);
// }
