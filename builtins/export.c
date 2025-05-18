#include "../includes/minishell.h"

int is_valid_env_name(const char *name)
{
    int i;

    if (!name || !*name)
        return (0);
    if (!(ft_isalpha(*name) || *name == '_'))
        return (0);
    i = 1;
    while (name[i] && name[i] != '=')
    {
        if (!(ft_isalnum(name[i]) || name[i] == '_'))
            return (0);
        i++;
    }
    return (1);
}

void update_env(t_env **env_list, char *name, char *value)
{
    t_env *current;
    t_env *new;

    current = *env_list;
    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }
    new = malloc(sizeof(t_env));
    if (!new)
        return;
    new->name = ft_strdup(name);
    new->value = ft_strdup(value);
    new->next = *env_list;
    *env_list = new;
}

void print_env_sorted(t_env *env_list)
{
    int count;
    int i;
    int j;
    t_env *temp;
    t_env **arr;
    t_env *swap;

    count = 0;
    temp = env_list;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    arr = malloc(count * sizeof(t_env *));
    if (!arr)
        return;
    temp = env_list;
    i = 0;
    while (i < count)
    {
        arr[i] = temp;
        temp = temp->next;
        i++;
    }
    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (ft_strcmp(arr[j]->name, arr[j + 1]->name) > 0)
            {
                swap = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = swap;
            }
            j++;
        }
        i++;
    }
    i = 0;
    while (i < count)
    {
        printf("declare -x %s=\"%s\"\n", arr[i]->name, arr[i]->value);
        i++;
    }
    free(arr);
}

int ft_export(t_env **env_list, char **args)
{
    int i;
    char *equal_sign;
    char *name;
    char *value;

    if (!args[1])
    {
        print_env_sorted(*env_list);
        return (0);
    }
    i = 1;
    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        name = args[i];
        value = "";
        if (equal_sign)
        {
            *equal_sign = '\0';
            value = equal_sign + 1;
        }
        if (!is_valid_env_name(name))
        {
            printf("minishell: export: `%s`: not a valid identifier\n", name);
            return (1);
        }
        update_env(env_list, name, value);
        i++;
    }
    return (0);
}

// // utils/env_utils.c
// bool is_valid_env_name(const char *name) {
//     if (!name || !*name)
//         return false;

//     // First char must be a letter or underscore
//     if (!(ft_isalpha(*name) || *name == '_'))
//         return false;

//     // Subsequent chars must be alphanumeric or underscore
//     while (*++name) {
//         if (!(ft_isalnum(*name) || *name == '_'))
//             return false;
//     }
//     return true;
// }

// // builtins/export.c
// void update_env(t_env **env_list, char *name, char *value) {
//     t_env *current = *env_list;

//     // Search for existing variable
//     while (current) {
//         if (ft_strcmp(current->name, name) == 0) {
//             free(current->value);
//             current->value = ft_strdup(value);
//             return;
//         }
//         current = current->next;
//     }

//     // If not found, add new entry
//     t_env *new = malloc(sizeof(t_env));
//     new->name = ft_strdup(name);
//     new->value = ft_strdup(value);
//     new->next = *env_list;
//     *env_list = new;
// }

// // builtins/export.c
// void print_env_sorted(t_env *env_list) {
//     if (!env_list)
//         return;

//     // Convert linked list to array for sorting
//     int count = 0;
//     t_env *temp = env_list;
//     while (temp && ++count) temp = temp->next;

//     t_env **arr = malloc(count * sizeof(t_env *));
//     temp = env_list;
//     for (int i = 0; i < count; i++) {
//         arr[i] = temp;
//         temp = temp->next;
//     }

//     // Bubble sort (simple but not optimal for large lists)
//     for (int i = 0; i < count - 1; i++) {
//         for (int j = 0; j < count - i - 1; j++) {
//             if (ft_strcmp(arr[j]->name, arr[j + 1]->name) > 0) {
//                 t_env *swap = arr[j];
//                 arr[j] = arr[j + 1];
//                 arr[j + 1] = swap;
//             }
//         }
//     }

//     // Print sorted list
//     for (int i = 0; i < count; i++)
//         printf("declare -x %s=\"%s\"\n", arr[i]->name, arr[i]->value);

//     free(arr);
// }

// // builtins/export.c
// int ft_export(t_env **env_list, char **args) {
//     if (!args[1]) {
//         print_env_sorted(*env_list); // If no args, print sorted env
//         return 0;
//     }

//     for (int i = 1; args[i]; i++) {
//         char *equal_sign = ft_strchr(args[i], '=');
//         char *name = args[i];
//         char *value = "";

//         if (equal_sign) {
//             *equal_sign = '\0'; // Split into name and value
//             value = equal_sign + 1;
//         }

//         if (!is_valid_env_name(name)) {
//             printf("minishell: export: `%s`: not a valid identifier\n", name);
//             return 1;
//         }

//         update_env(env_list, name, value);
//     }
//     return 0;
// }




// // utils/env_utils.c
// bool is_valid_env_name(const char *name) {
//     if (!name || !*name)
//         return false;
    
//     // First char must be a letter or underscore
//     if (!(ft_isalpha(*name) || *name == '_'))
//         return false;
    
//     // Subsequent chars must be alphanumeric or underscore
//     while (*++name) {
//         if (!(ft_isalnum(*name) || *name == '_'))
//             return false;
//     }
//     return true;
// }

// // builtins/export.c
// void update_env(t_env **env_list, char *name, char *value) {
//     t_env *current = *env_list;
    
//     // Search for existing variable
//     while (current) {
//         if (ft_strcmp(current->name, name) == 0) {
//             free(current->value);
//             current->value = ft_strdup(value);
//             return;
//         }
//         current = current->next;
//     }
    
//     // If not found, add new entry
//     t_env *new = malloc(sizeof(t_env));
//     new->name = ft_strdup(name);
//     new->value = ft_strdup(value);
//     new->next = *env_list;
//     *env_list = new;
// }

// // builtins/export.c
// void print_env_sorted(t_env *env_list) {
//     if (!env_list)
//         return;
    
//     // Convert linked list to array for sorting
//     int count = 0;
//     t_env *temp = env_list;
//     while (temp && ++count) temp = temp->next;
    
//     t_env **arr = malloc(count * sizeof(t_env *));
//     temp = env_list;
//     for (int i = 0; i < count; i++) {
//         arr[i] = temp;
//         temp = temp->next;
//     }
    
//     // Bubble sort (simple but not optimal for large lists)
//     for (int i = 0; i < count - 1; i++) {
//         for (int j = 0; j < count - i - 1; j++) {
//             if (ft_strcmp(arr[j]->name, arr[j + 1]->name) > 0) {
//                 t_env *swap = arr[j];
//                 arr[j] = arr[j + 1];
//                 arr[j + 1] = swap;
//             }
//         }
//     }
    
//     // Print sorted list
//     for (int i = 0; i < count; i++)
//         printf("declare -x %s=\"%s\"\n", arr[i]->name, arr[i]->value);
    
//     free(arr);
// }

// // builtins/export.c
// int ft_export(t_env **env_list, char **args) {
//     if (!args[1]) {
//         print_env_sorted(*env_list); // If no args, print sorted env
//         return 0;
//     }
    
//     for (int i = 1; args[i]; i++) {
//         char *equal_sign = ft_strchr(args[i], '=');
//         char *name = args[i];
//         char *value = "";
        
//         if (equal_sign) {
//             *equal_sign = '\0'; // Split into name and value
//             value = equal_sign + 1;
//         }
        
//         if (!is_valid_env_name(name)) {
//             printf("minishell: export: `%s`: not a valid identifier\n", name);
//             return 1;
//         }
        
//         update_env(env_list, name, value);
//     }
//     return 0;
// }
