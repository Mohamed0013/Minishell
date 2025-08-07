/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:40 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:40 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

static void	extract_var_name(const char *arg, size_t start, char *var_name)
{
	size_t	var_len;

	var_len = 0;
	while (ft_isalnum(arg[start + var_len]) || arg[start + var_len] == '_')
		var_len++;
	ft_strlcpy(var_name, arg + start, var_len + 1);
}

int	handle_variable_expansion(t_expand_ctx *ctx)
{
	size_t		var_start;
	char		var_name[1024];
	const char	*val;
	size_t		vlen;
	size_t		var_len;

	if (ctx->in_squote || (!ft_isalpha(ctx->arg[ctx->i + 1]) && ctx->arg[ctx->i
				+ 1] != '_'))
		return (0);
	var_start = ctx->i + 1;
	var_len = 0;
	while (ft_isalnum(ctx->arg[var_start + var_len]) || ctx->arg[var_start
			+ var_len] == '_')
		var_len++;
	extract_var_name(ctx->arg, var_start, var_name);
	val = get_env_value(ctx->env, var_name);
	vlen = ft_strlen(val);
	ft_memcpy(ctx->result + ctx->j, val, vlen);
	ctx->j += vlen;
	ctx->i = var_start + var_len;
	return (1);
}
