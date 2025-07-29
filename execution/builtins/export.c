#include "../includes/minishell.h"

int	is_valid_env_name(const char *name)
{
	int	i;

	i = 1;
	if (!name || !*name)
		return (0);
	if (!(ft_isalpha(*name) || *name == '_'))
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	update_env(t_env **env_list, char *name, char *value)
{
	t_env	*current;
	t_env	*new;

	current = *env_list;
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
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = *env_list;
	*env_list = new;
}

void	fill_one(int i, int count, t_env *temp, t_env **arr)
{
	while (i < count)
	{
		arr[i] = temp;
		temp = temp->next;
		i++;
	}
}

void	fill_arr(t_env **arr, t_env *temp, t_env *swap, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	fill_one(i, count, temp, arr);
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
}

t_env	**utils(int count, t_env *temp, t_env *env_list)
{
	t_env	**arr;
	t_env	*swap;

	swap = NULL;
	arr = malloc(count * sizeof(t_env *));
	if (!arr)
		return (NULL);
	temp = env_list;
	fill_arr(arr, temp, swap, count);
	return (arr);
}

void	print_util(int i, int count, t_env **arr)
{
	while (i < count)
	{
		if (arr[i]->value)
			printf("declare -x %s=\"%s\"\n", arr[i]->name, arr[i]->value);
		else
			printf("declare -x %s\n", arr[i]->name);
		i++;
	}
}

void	print_env_sorted(t_env *env_list)
{
	int		count;
	t_env	*temp;
	t_env	**arr;

	count = 0;
	temp = env_list;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	arr = utils(count, temp, env_list);
	if (!arr)
		return ;
	print_util(0, count, arr);
	free(arr);
}

void	check_and_free(char *value, char *name)
{
	if (value)
		free(value);
	if (name)
		free(name);
}

int	ft_export(t_env **env, char **args)
{
	int		i;
	int		ret;
	char	*name;
	char	*value;

	i = 1;
	ret = 0;
	if (!args || !args[1])
	{
		print_env_sorted(*env);
		return (0);
	}
	while (args[i])
	{
		name = NULL;
		value = NULL;
		if (validate_and_split(args[i], &name, &value))
			ret = 1;
		else
			update_or_add_env(env, name, value);
		check_and_free(value, name);
		i++;
	}
	return (ret);
}
