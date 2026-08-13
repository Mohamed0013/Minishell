/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:38 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:38 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

char	*expand_arg(const char *arg, t_env *env)
{
	t_expand_ctx	ctx;
	size_t			len;
	size_t			max_expand_len;

	if (!arg || !*arg || !env)
		return (NULL);
	len = ft_strlen(arg);
	max_expand_len = len * 32 + 1;
	ctx.result = ft_malloc(max_expand_len);
	if (!ctx.result)
		return (NULL);
	init_expand_ctx(&ctx, arg, env);
	while (ctx.arg[ctx.i])
	{
		if (ctx.arg[ctx.i] == '\'' && handle_single_quote(&ctx))
			continue ;
		if (ctx.arg[ctx.i] == '"' && handle_double_quote(&ctx))
			continue ;
		if (ctx.arg[ctx.i] == '$' && handle_variable_expansion(&ctx))
			continue ;
		ctx.result[ctx.j++] = ctx.arg[ctx.i++];
	}
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}
