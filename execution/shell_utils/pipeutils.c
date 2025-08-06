#include "../../includes/minishell.h"

static int	handle_child_process(t_ast *current, t_execute *exec, int i,
		char **env)
{
	t_child_data	data;

	data.full_command = get_args(current->args);
	if (!data.full_command)
		ft_exit_withclear(1);
	setup_child_pipes(exec, i, exec->nb_pipes, current->redirections);
	if (current->redirections
		&& handle_redirections(current->redirections) != 0)
		ft_exit_withclear(1);
	if (is_builtin(data.full_command[0]))
		execute_child_builtin(&data, current, exec);
	else
		execute_child_external_pipe(&data, env);
	return (0);
}

static int	check_exit_in_pipeline(t_ast *ast)
{
	char	**first_cmd;

	first_cmd = get_args(ast->args);
	if (first_cmd && strcmp(first_cmd[0], "exit") == 0)
		return (2);
	return (0);
}

static int	wait_for_processes(pid_t *pids, int count, t_execute *exec)
{
	int	j;
	int	status;
	int	last_status;

	j = 0;
	last_status = 0;
	while (j < count)
	{
		waitpid(pids[j], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			last_status = 128 + WTERMSIG(status);
			if ((last_status == 128 + SIGINT || last_status == 128 + SIGQUIT)
				&& j == count - 1)
				write(1, "\n", 1);
		}
		else if (WIFSTOPPED(status))
			last_status = 128 + WSTOPSIG(status);
		j++;
	}
	exec->exit_status = last_status;
	return (exec->exit_status);
}

static int	initialize_pipe_data(t_pipe_data *data, t_ast *ast, int nb_pipes,
		t_execute *exec)
{
	data->i = 0;
	data->current = ast;
	if (set_pipefds(nb_pipes, exec))
		return (1);
	data->exit_check = check_exit_in_pipeline(ast);
	if (data->exit_check == 2)
		return (2);
	data->pids = ft_malloc((nb_pipes + 1) * sizeof(pid_t));
	if (!data->pids)
		return (1);
	return (0);
}

int	handle_pipes(t_ast *ast, int nb_pipes, t_execute *exec, char **env)
{
	t_pipe_data	data;
	int			init_result;

	init_result = initialize_pipe_data(&data, ast, nb_pipes, exec);
	if (init_result != 0)
		return (init_result);
	while (data.current)
	{
		data.pids[data.i] = fork();
		if (data.pids[data.i] < 0)
		{
			perror("Fork failed");
			free(data.pids);
			return (1);
		}
		else if (data.pids[data.i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			handle_child_process(data.current, exec, data.i, env);
		}
		close_parent_pipes(exec, data.i, nb_pipes);
		data.current = data.current->next;
		data.i++;
	}
	return (wait_for_processes(data.pids, data.i, exec));
}
