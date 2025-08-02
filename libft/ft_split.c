/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:22:12 by mohdahma          #+#    #+#             */
/*   Updated: 2024/11/04 21:25:50 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_strings(char const *s, char sep)
{
	size_t	count;
	int		i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == sep)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != sep)
			i++;
	}
	return (count);
}

static unsigned int	word_len(char const *s, char sep)
{
	unsigned int	i;

	i = 0;
	while (s[i] && s[i] != sep)
		i++;
	return (i);
}

static char	**free_strings_array(char **splited)
{
	size_t	i;

	i = 0;
	while (splited[i])
	{
		free(splited[i]);
		i++;
	}
	free(splited);
	return (NULL);
}

static char	**ft_make_fill_arrays(char **splited, char const *s, char c)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	len;

	i = 0;
	j = 0;
	while (s[i])
	{
		len = word_len(s + i, c);
		if (len > 0)
		{
			splited[j] = ft_substr(s, i, len);
			if (!splited[j])
				return (free_strings_array(splited));
			j++;
			i += len;
		}
		else
			i++;
	}
	splited[j] = NULL;
	return (splited);
}

char	**ft_split(char const *s, char c)
{
	char	**splited;

	if (!s)
		return (NULL);
	splited = ft_calloc((ft_count_strings(s, c) + 1), sizeof(char *));
	if (!splited)
		return (NULL);
	return (ft_make_fill_arrays(splited, s, c));
}
