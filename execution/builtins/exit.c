#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	int	exit_code;

	exit_code = g_data.exit_status;
	ft_putstr_fd("exit\n", 2);
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit_code = 2;
		}
		else
		{
			exit_code = ft_atoi(args[1]);
			exit_code = exit_code & 255;
		}
		if (args[2])
			return (ft_putstr_fd("minishell: exit:\
			too many arguments\n", 2), 1);
	}
	ft_exit_withclear(exit_code);
	return (exit_code);
}
