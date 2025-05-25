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
        return ;
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
        if (arr[i]->value)
            printf("declare -x %s=\"%s\"\n", arr[i]->name, arr[i]->value);
        else
            printf("declare -x %s\n", arr[i]->name);
        i++;
    }
    free(arr);
}

int ft_export(t_env **env, char **args)
{
    int i = 1;
    int ret = 0;
    char *name;
    char *value;

    if (!args || !args[1])
    {
        print_env_sorted(*env);
        return 0;
    }
    while (args[i])
    {
        name = NULL;
        value = NULL;
        if (validate_and_split(args[i], &name, &value))
        {
            // Invalid identifier, error already printed by validate_and_split
            ret = 1;
        }
        else
        {
            update_or_add_env(env, name, value);
            // update_or_add_env takes ownership of name and value
        }
        i++;
    }
    return ret;
}

// int validate_and_split(char *arg, char **name, char **value)
// {
//     char *eq_pos;

//     eq_pos = ft_strchr(arg, '=');
//     if (eq_pos)
//     {
//         *name = ft_substr(arg, 0, eq_pos - arg);
//         *value = ft_strdup(eq_pos + 1);
//     }
//     else
//     {
//         *name = ft_strdup(arg);
//         *value = ft_strdup("");
//     }
//     if (!is_valid_env_name(*name))
//     {
//         ft_putstr_fd("minishell: export: `", STDERR_FILENO);
//         ft_putstr_fd(arg, STDERR_FILENO);
//         ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
//         free(*name);
//         free(*value);
//         return (1);
//     }
//     return (0);
// }

// void update_or_add_env(t_env **env, char *name, char *value)
// {
//     t_env *current;

//     current = *env;
//     while (current)
//     {
//         if (ft_strcmp(current->name, name) == 0)
//         {
//             free(current->value);
//             current->value = ft_strdup(value);
//             free(name);
//             free(value);
//             return;
//         }
//         current = current->next;
//     }
//     lst_add_env(env, lst_new_env(name, value));
// }
