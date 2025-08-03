#include "minishell.h"

int	ft_exit_withclear(int code)
{
	t_env	*env;

	env = g_data.env_list;
	ft_gc_clear();
	free_env_list(env);
	exit(code);
}
