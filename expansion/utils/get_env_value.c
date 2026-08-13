/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:39 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:39 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

char	*get_env_value(t_env *env, char *var_name)
{
	if (!env || !var_name || !*var_name)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->name) == ft_strlen(var_name) && ft_strncmp(env->name,
				var_name, ft_strlen(var_name)) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}
