/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:59:53 by mohdahma          #+#    #+#             */
/*   Updated: 2024/10/28 10:59:53 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	char	*s2;
	int		len;

	if (!s)
		return (NULL);
	len = 0;
	while (s[len])
		len++;
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	s2 = str;
	while (*s)
		*s2++ = *s++;
	*s2 = '\0';
	return (str);
}
