// export_utils.c
#include "../includes/minishell.h"

t_env *lst_new_env(char *name, char *value)
{
    t_env *new;

    new = malloc(sizeof(t_env));
    if (!new)
        return NULL;
    new->name = ft_strdup(name);
    new->value = ft_strdup(value);
    new->next = NULL;
    return new;
}

void lst_add_env(t_env **lst, t_env *new)
{
    t_env *cur;

    if (!lst || !new)
        return;
    if (!*lst)
    {
        *lst = new;
        return;
    }
    cur = *lst;
    while (cur->next)
        cur = cur->next;
    cur->next = new;
}

int validate_and_split(char *arg, char **name, char **value)
{
    char *eq_pos;

    eq_pos = ft_strchr(arg, '=');
    if (eq_pos)
    {
        *name = ft_substr(arg, 0, eq_pos - arg);
        *value = ft_strdup(eq_pos + 1);
    }
    else
    {
        *name = ft_strdup(arg);
        // *value = ft_strdup("");
        *value = NULL;
    }
    if (!is_valid_env_name(*name))
    {
        ft_putstr_fd("minishell: export: `", STDERR_FILENO);
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
        free(*name);
        free(*value);
        return (1);
    }
    return (0);
}

void update_or_add_env(t_env **env, char *name, char *value)
{
    t_env *current;

    current = *env;
    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            free(name);
            free(value);
            return;
        }
        current = current->next;
    }
    t_env *new = lst_new_env(name, value);
    if (!new)
    {
        free(name);
        free(value);
        return;
    }
    lst_add_env(env, new);
}

int ft_unset(t_env **env, char **args)
{
    int i = 1;
    t_env *current;
    t_env *prev;
    t_env *tmp;

    if (!args)
        return (0);

    while (args[i])
    {
        prev = NULL;
        current = *env;
        while (current)
        {
            if (ft_strcmp(current->name, args[i]) == 0)
            {
                tmp = current->next;
                if (prev)
                    prev->next = tmp;
                else
                    *env = tmp;
                free(current->name);
                free(current->value);
                free(current);
                break;
            }
            prev = current;
            current = current->next;
        }
        i++;
    }
    return (0);
}
