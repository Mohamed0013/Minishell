/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <moel-yag@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:33 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/10 15:44:33 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_outer_quotes(const char *str)
{
	size_t	len;
	char	*result;

	if (!str)
		return (NULL);
	len = strlen(str);
	if (len >= 2
		&& ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"')))
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		memcpy(result, str + 1, len - 2);
		result[len - 2] = '\0';
		return (result);
	}
	return (strdup(str));
}
