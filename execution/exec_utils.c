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
	(void) env;
	data->ret = execute_single_command(data, ast, current_env);
	// if (current_env != env)
	// 	free_split(current_env);
	return (1);
}
