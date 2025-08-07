/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:35 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:35 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_and_free(char *value, char *name)
{
	if (value)
		free(value);
	if (name)
		free(name);
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
