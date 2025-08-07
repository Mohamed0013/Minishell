/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_qotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:40 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:40 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

int	handle_single_quote(t_expand_ctx *ctx)
{
	if (!ctx->in_dquote)
	{
		ctx->in_squote = !ctx->in_squote;
		ctx->i++;
		return (1);
	}
	return (0);
}

int	handle_double_quote(t_expand_ctx *ctx)
{
	if (!ctx->in_squote)
	{
		ctx->in_dquote = !ctx->in_dquote;
		ctx->i++;
		return (1);
	}
	return (0);
}
