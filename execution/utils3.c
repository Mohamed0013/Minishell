#include "../includes/minishell.h"

void	exec_error(int err, char **cmd)
{
	if (err == ENOENT)
	{
		printf("minishell: %s: command not found\n", cmd[0]);
		exit(127);
	}
	else if (err == EACCES)
	{
		printf("minishell: %s: Permission denied\n", cmd[0]);
		exit(126);
	}
	else
	{
		perror("minishell");
		exit(1);
	}
}

static void	execute_child_external(
	t_external_data *data, char **cmd, t_list *redir, char **env)
{
	int err;

	if (redir && handle_redirections(redir) != 0)
	{
		free_split(cmd);
		exit(1);
	}
	if (str_ichr(cmd[0], '/') > -1)
		data->path = cmd[0];
	else
		data->path = get_path(cmd[0], env);
	if (execve(data->path, cmd, env) == -1)
	{
		err = errno;
		exec_error(err, cmd);
		free_split(cmd);
	}
}

static int	execute_external_cmd(
	t_execute *exec, char **cmd, t_list *redir, char **env)
{
	t_external_data	data;

	data.pid = fork();
	if (data.pid < 0)
	{
		perror("Fork failed");
		return (-1);
	}
	else if (data.pid == 0)
		execute_child_external(&data, cmd, redir, env);
	waitpid(data.pid, &data.status, 0);
	if (WIFEXITED(data.status))
		exec->exit_status = WEXITSTATUS(data.status);
	return (exec->exit_status);
}

int	execute_command(t_execute *exec, char **cmd, t_list *redir, char **env)
{
	int	ret;

	if (is_builtin(cmd[0]))
	{
		if (exec->nb_pipes > 0)
			return (execute_builtin_piped(exec, cmd, redir));
		else
		{
			ret = execute_builtin_direct(exec, cmd, redir);
			if (exec->exit_status == 2)
				return (2);
			return (ret);
		}
	}
	else
	{
		return (execute_external_cmd(exec, cmd, redir, env));
	}
}