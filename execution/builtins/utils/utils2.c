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
		return (2);
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
