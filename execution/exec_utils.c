/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:14 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:16 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_utils(t_exec_data *data, t_ast *ast, int status)
{
	data->exec->exit_status = status;
	data->exec->nb_pipes = 0;
	data->exec->pipfds = NULL;
	data->nb_pipes = count_pipes(ast);
}

int	exec_utils2(t_exec_data *data, t_ast *ast, char **current_env, char **env)
{
	(void)env;
	data->ret = execute_single_command(data, ast, current_env);
	return (1);
}

void	exec_error(int err, char **cmd)
{
	if (err == ENOENT)
	{
		printf("minishell: %s: command not found\n", cmd[0]);
		ft_exit_withclear(127);
	}
	else if (err == EACCES)
	{
		printf("minishell: %s: Permission denied\n", cmd[0]);
		ft_exit_withclear(126);
	}
	else
	{
		perror("minishell");
		ft_exit_withclear(1);
	}
}
