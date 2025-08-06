#include "../includes/minishell.h"

t_env	*lst_new_env(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

void	lst_add_env(t_env **lst, t_env *new)
{
	t_env	*cur;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

void	update_or_add_env(t_env **env, char *name, char *value)
{
	t_env	*current;
	t_env	*new;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new = lst_new_env(name, value);
	if (!new)
		return ;
	lst_add_env(env, new);
}

void	ft_swap(t_env *current, t_env *prev, t_env **env)
{
	t_env	*tmp;

	tmp = current->next;
	if (prev)
		prev->next = tmp;
	else
		*env = tmp;
	free(current->name);
	free(current->value);
	free(current);
}

int	ft_unset(t_env **env, char **args)
{
	t_env	*current;
	t_env	*prev;

	int (i) = 1;
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
				ft_swap(current, prev, env);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}
