/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:27 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:27 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_exec(t_execute *exec)
{
	int	i;

	i = 0;
	if (!exec)
		return ;
	if (exec->pipfds)
	{
		while (i < exec->nb_pipes)
		{
			if (exec->pipfds[i])
			{
				close(exec->pipfds[i][0]);
				close(exec->pipfds[i][1]);
			}
			i++;
		}
	}
}

int	execute_single_command(t_exec_data *data, t_ast *ast, char **env)
{
	data->full_command = get_args(ast->args);
	if (!data->full_command)
		return (1);
	data->ret = execute_command(data->exec, data->full_command,
			ast->redirections, env);
	return (data->ret);
}
