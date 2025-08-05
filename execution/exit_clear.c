#include "minishell.h"

int	ft_exit_withclear(int code)
{
	if (g_data.env_list)
	{
		free_env_list(g_data.env_list);
		g_data.env_list = NULL;
	}
	ft_gc_clear();
	exit(code);
}
