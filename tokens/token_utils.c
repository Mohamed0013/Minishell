/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:46 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:46 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

int	condition3(t_ddata *ddata, const char *input, int *i, int in_quote)
{
	t_token	*new_token;
	char	*substr;

	if (ddata->len > 0)
	{
		substr = ft_substr(input, *i, ddata->len);
		if (!substr)
			return (0);
		new_token = create_token(substr, TOKEN_WORD, in_quote);
		ft_gc_add(substr);
		if (!new_token)
			return (0);
		add_token(&ddata->head, new_token);
		*i += ddata->len;
		ddata->len = 0;
	}
	while (ddata->ptr[*i] == ' ' || ddata->ptr[*i] == '\t')
		(*i)++;
	return (1);
}

int	condition2(t_ddata *ddata, int *i, int *in_quote)
{
	char	quote_char;

	if (ddata->ptr[*i + ddata->len] == '\'')
		*in_quote = 1;
	else
		*in_quote = 2;
	quote_char = ddata->ptr[*i + ddata->len];
	ddata->len++;
	while (ddata->ptr[*i + ddata->len] && ddata->ptr[*i
			+ ddata->len] != quote_char)
		ddata->len++;
	if (ddata->ptr[*i + ddata->len] == quote_char)
		ddata->len++;
	return (1);
}

int	condition1(t_ddata *ddata, const char *input, int *i, int in_quote)
{
	t_token	*new_token;
	t_token	*eof_token;
	char	*substr;

	if (ddata->len > 0)
	{
		substr = ft_substr(input, *i, ddata->len);
		if (!substr)
			return (0);
		new_token = create_token(substr, TOKEN_WORD, in_quote);
		ft_gc_add(substr);
		if (!new_token)
		{
			return (0);
		}
		add_token(&ddata->head, new_token);
	}
	eof_token = create_token(NULL, TOKEN_EOF, 0);
	if (!eof_token)
		return (0);
	add_token(&ddata->head, eof_token);
	return (1);
}

bool	unclosed_quotes(const char *input)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	return (in_single_quote || in_double_quote);
}
