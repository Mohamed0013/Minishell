/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:11:10 by mohdahma          #+#    #+#             */
/*   Updated: 2024/10/28 11:11:10 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_get_length(int n)
{
	int		count;
	long	nb;

	count = 0;
	nb = n;
	if (nb == 0)
		return (1);
	else if (nb < 0)
	{
		count++;
		nb = -nb;
	}
	while (nb > 0)
	{
		nb /= 10;
		count++;
	}
	return (count);
}

static void	ft_fill_string(char *result, int len, long n)
{
	if (n == 0)
		result[0] = '0';
	while (n > 9)
	{
		result[--len] = (n % 10) + '0';
		n /= 10;
	}
	result[--len] = n + '0';
}

char	*ft_itoa(int n)
{
	char	*result;
	int		len;
	long	nb;

	len = ft_get_length(n);
	nb = n;
	result = malloc(sizeof(char) * len + 1);
	if (!result)
		return (NULL);
	if (nb < 0)
	{
		result[0] = '-';
		nb = -nb;
	}
	result[len] = '\0';
	ft_fill_string(result, len, nb);
	return (result);
}
