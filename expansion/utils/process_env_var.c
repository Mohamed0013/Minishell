/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_env_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:41 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:41 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

int	process_env_variable(char **value, int *len)
{
	int		i;
	char	c;
	char	*env_value;

	(*value)++;
	i = 0;
	while (isalnum((*value)[i]) || (*value)[i] == '_')
		i++;
	c = (*value)[i];
	(*value)[i] = '\0';
	env_value = getenv(*value);
	*len += strlen(env_value);
	(*value)[i] = c;
	*value += i;
	return (0);
}
