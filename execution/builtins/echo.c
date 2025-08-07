/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <moel-yag@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:32 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:55:39 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	empty_arg(char **arguments)
{
	if (!arguments || !arguments[0])
	{
		printf("\n");
		return (1);
	}
	return (0);
}

static int	is_arg_n(char *arg)
{
	int	i;

	if (!arg || !*arg)
		return (0);
	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	execute_echo(char **arguments, int status)
{
	int (i) = 1;
	int (newline) = 1;
	if (empty_arg(arguments) == 1)
		return ;
	if (is_arg_n(arguments[i]) == 1)
	{
		newline = 0;
		i++;
	}
	else if (strcmp(arguments[i], "$?") == 0)
	{
		printf("%d", status);
		if (newline)
			printf("\n");
		return ;
	}
	while (arguments[i])
	{
		printf("%s", arguments[i]);
		if (arguments[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
