/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:47 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:47 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

int	first_peace(t_ddata *ddata, const char *input, int in_quote, int *i)
{
	if (ddata->ptr[*i + ddata->len] == '\0')
	{
		if (!condition1(ddata, input, i, in_quote))
			return (1);
		return (0);
	}
	if ((ddata->ptr[*i + ddata->len] == '\'' || ddata->ptr[*i
				+ ddata->len] == '\"') && (condition2(ddata, i, &in_quote)))
		return (2);
	if ((ddata->ptr[*i + ddata->len] == ' ' || ddata->ptr[*i
				+ ddata->len] == '\t'))
	{
		if (!condition3(ddata, input, i, in_quote))
			return (1);
		return (2);
	}
	if (ddata->ptr[*i + ddata->len] == '|')
	{
		if (!condition4(ddata, input, i, in_quote))
			return (1);
		return (2);
	}
	return (-1);
}

int	second_peace(t_ddata *ddata, const char *input, int in_quote, int *i)
{
	if (ddata->ptr[*i + ddata->len] == '<')
	{
		if (!condition5(ddata, input, i, in_quote))
			return (1);
		return (2);
	}
	if (ddata->ptr[*i + ddata->len] == '>')
	{
		if (!condition6(ddata, input, i, in_quote))
			return (1);
		return (2);
	}
	ddata->len++;
	return (0);
}

t_token	*tokenize_loop(t_ddata *ddata, const char *input)
{
	int	i;
	int	in_quote;
	int	ret;

	i = 0;
	in_quote = 0;
	while (1)
	{
		ret = first_peace(ddata, input, in_quote, &i);
		if (ret == 0)
			break ;
		else if (ret == 1)
			return (NULL);
		else if (ret == 2)
			continue ;
		ret = second_peace(ddata, input, in_quote, &i);
		if (ret == 1)
			return (NULL);
		else if (ret == 2)
			continue ;
	}
	return (ddata->head);
}

t_token	*tokenize_input(const char *input)
{
	t_ddata	*ddata;
	t_token	*head;

	ddata = ft_malloc(sizeof(t_ddata));
	if (!ddata)
		return (NULL);
	ddata->head = NULL;
	ddata->len = 0;
	ddata->ptr = input;
	head = tokenize_loop(ddata, input);
	return (head);
}

t_token	*tokenize(const char *input)
{
	t_token	*tokens;

	if (!input || !*input)
		return (NULL);
	if (unclosed_quotes(input))
	{
		ft_putstr_fd("minishell: unclosed quotes\n", 2);
		return (NULL);
	}
	tokens = tokenize_input(input);
	if (!tokens)
	{
		ft_putstr_fd("minishell: error tokenizing input\n", 2);
		return (NULL);
	}
	return (tokens);
}
