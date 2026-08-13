/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:57:09 by mohdahma          #+#    #+#             */
/*   Updated: 2025/08/10 15:08:09 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	is_builtin(char *command)
{
	return (ft_strcmp(command, "cd") == 0 || ft_strcmp(command, "echo") == 0
		|| ft_strcmp(command, "export") == 0 || ft_strcmp(command, "env") == 0
		|| ft_strcmp(command, "exit") == 0 || ft_strcmp(command, "unset") == 0
		|| ft_strcmp(command, "pwd") == 0);
}

int	exec_builtin_part2(char **args, t_list *redirections)
{
	if (ft_strcmp(args[0], "env") == 0)
	{
		ft_env(g_data.env_list, args);
		return (0);
	}
	else if (ft_strcmp(args[0], "unset") == 0)
	{
		ft_unset(&g_data.env_list, args);
		return (0);
	}
	else if (ft_strcmp(args[0], "pwd") == 0)
	{
		execute_pwd();
		return (0);
	}
	else if (ft_strcmp(args[0], "exit") == 0)
	{
		if (redirections)
			ft_lstclear(&redirections, free);
		return (ft_exit(args));
	}
	return (-1);
}

int	exec_builtin_part1(char **args, int exit_status)
{
	if (ft_strcmp(args[0], "cd") == 0)
	{
		execute_cd(args);
		return (0);
	}
	else if (ft_strcmp(args[0], "echo") == 0)
	{
		execute_echo(args, exit_status);
		return (0);
	}
	else if (ft_strcmp(args[0], "export") == 0)
	{
		ft_export(&g_data.env_list, args);
		return (0);
	}
	return (-1);
}

void	print_cd_error(char *arg)
{
	char	*part1;
	char	*part2;
	char	*part3;
	char	*part4;
	char	*final_msg;

	part1 = ft_strjoin("minishell: cd: ", arg);
	part2 = ft_strjoin(part1, ": ");
	free(part1);
	part3 = ft_strjoin(part2, strerror(errno));
	free(part2);
	part4 = ft_strjoin(part3, "\n");
	free(part3);
	final_msg = part4;
	write(2, final_msg, strlen(final_msg));
	free(final_msg);
}
