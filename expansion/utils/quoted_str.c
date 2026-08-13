/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:42 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:42 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

int	process_quoted_string(char **value, int *len)
{
	char	c;

	c = **value;
	(*value)++;
	while (**value && **value != c)
	{
		(*len)++;
		(*value)++;
	}
	if (**value == c)
		(*value)++;
	return (0);
}
