/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:58:27 by mohdahma          #+#    #+#             */
/*   Updated: 2024/11/04 20:58:08 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	d_len;
	size_t	s_len;

	s_len = ft_strlen(src);
	if (!dst && !dstsize)
		return (s_len);
	d_len = ft_strlen(dst);
	if (dstsize > d_len)
		ft_strlcpy(dst + d_len, src, dstsize - d_len);
	if (dstsize < d_len)
		return (dstsize + s_len);
	return (d_len + s_len);
}
