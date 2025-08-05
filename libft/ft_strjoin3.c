/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <moel-yag@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:35:00 by mohdahma          #+#    #+#             */
/*   Updated: 2025/08/04 10:56:12 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	char	*new;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (s1 == NULL && s2 == NULL && s3 == NULL)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	new = malloc(sizeof(char) * (len1 + len2 + len3 + 1));
	if (new == NULL)
		return (NULL);
	ft_strlcpy(new, s1, len1 + 1);
	ft_strlcpy(new + len1, s2, len2 + 1);
	ft_strlcpy(new + len1 + len2, s3, len3 + 1);
	return (new);
}
