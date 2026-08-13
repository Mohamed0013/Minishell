/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:37 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:37 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "expansion.h"

void	expand(t_env *env, t_token *token)
{
	t_token	*current;
	char	*expanded;

	if (!env || !token)
		return ;
	current = token;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD && (!current->prev
				|| current->prev->type != TOKEN_HEREDOC))
		{
			expanded = expand_arg(current->value, env);
			if (expanded)
			{
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
