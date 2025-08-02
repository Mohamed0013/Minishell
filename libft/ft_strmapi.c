/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:56:07 by mohdahma          #+#    #+#             */
/*   Updated: 2024/10/28 10:56:07 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*func)(unsigned int, char))
{
	char	*result;
	int		i;

	i = 0;
	if (s == NULL || func == NULL)
		return (NULL);
	i = ft_strlen(s);
	result = malloc(sizeof(char) * i + 1);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		result[i] = func(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
