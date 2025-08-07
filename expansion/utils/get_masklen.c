/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_masklen.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:39 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:39 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

int	mask_len(t_token *token)
{
	int		len;
	char	*value;

	if (!token || !token->value)
		return (0);
	len = 0;
	value = token->value;
	while (*value)
	{
		if (*value == '\'' || *value == '"')
			process_quoted_string(&value, &len);
		else if (*value == '$' && (isalpha(*(value + 1)) || *(value
					+ 1) == '_'))
			process_env_variable(&value, &len);
		else
		{
			len++;
			value++;
		}
	}
	return (len);
}
