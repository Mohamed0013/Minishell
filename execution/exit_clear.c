/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_clear.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:22 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:22 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit_withclear(int code)
{
	t_env	*env;

	env = g_data.env_list;
	ft_gc_clear();
	free_env_list(env);
	exit(code);
}
