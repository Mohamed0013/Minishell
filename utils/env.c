/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:36:31 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:50 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	*ft_strchr(const char *s, int c)
// {
// 	int	i;

// 	i = 0;
// 	while ((char)c != s[i])
// 	{
// 		if (!s[i])
// 			return (NULL);
// 		i++;
// 	}
// 	return ((char *)&s[i]);
// }

char	*str_ndup(char *str, unsigned int n)
{
	char			*s;
	unsigned int	i;

	i = 0;
	s = malloc(sizeof(char) * (n + 1));
	while (i < n)
		s[i++] = *str++;
	s[n] = 0;
	return (s);
}

t_env	*env_from_array(char **env)
{
	t_env	*head;
	t_env	*new;
	int		i;
	char	*eq;

	head = NULL;
	i = 0;
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			new = malloc(sizeof(t_env));
			new->name = str_ndup(env[i], eq - env[i]);
			new->value = ft_strdup(eq + 1);
			new->next = head;
			head = new;
		}
		i++;
	}
	return (head);
}

int	get_count(t_env *current)
{
	int	count;

	count = 0;
	while (current)
	{
		if (current->name && current->value)
			count++;
		current = current->next;
	}
	return (count);
}

int	loop_env(t_env *current, char **env_array, int count)
{
	char	*temp;

	int (i) = 0;
	while (current && i < count)
	{
		if (current->name && current->value)
		{
			temp = ft_strjoin(current->name, "=");
			if (!temp)
				return (1);
			env_array[i] = ft_strjoin(temp, current->value);
			ft_gc_add(env_array[i]);
			free(temp);
			if (!env_array[i])
				return (1);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (0);
}

char	**env_to_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;

	current = env_list;
	count = get_count(current);
	env_array = malloc(sizeof(char *) * (count + 1));
	ft_gc_add(env_array);
	if (!env_array)
		return (NULL);
	current = env_list;
	if (loop_env(current, env_array, count) == 1)
		return (NULL);
	return (env_array);
}
