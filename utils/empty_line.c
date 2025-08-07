/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:48 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:48 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_isspace(unsigned char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

bool	is_blank_line(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (true);
	while (s[i])
	{
		if (!ft_isspace((unsigned char)s[i]))
			return (false);
		i++;
	}
	return (true);
}
