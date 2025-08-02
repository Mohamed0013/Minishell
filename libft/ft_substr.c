/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:51:42 by mohdahma          #+#    #+#             */
/*   Updated: 2024/10/28 10:51:42 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	f_len;
	size_t	s_len;
	char	*new;

	i = 0;
	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	f_len = s_len - start;
	if (start > s_len || !len)
		return (ft_strdup(""));
	if (len > f_len)
		len = f_len;
	new = ft_calloc((len + 1), sizeof(char));
	if (!new)
		return (NULL);
	while (i < len)
	{
		new[i] = s[start];
		start++;
		i++;
	}
	return (new);
}
