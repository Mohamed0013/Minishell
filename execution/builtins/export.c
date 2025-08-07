/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:33 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:33 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	print_env_sorted(t_env *env_list)
{
	int		count;
	t_env	*temp;
	t_env	**arr;

	if (!env_list)
		return ;
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

int	ft_export(t_env **env, char **args)
{
	char	*name;
	char	*value;

	int (i), (ret);
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
		{
			update_or_add_env(env, name, value);
			check_and_free(value, name);
		}
		i++;
	}
	return (ret);
}
