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
	int	err;

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

int	validate_and_split(char *arg, char **name, char **value)
{
	char	*eq_pos;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
	{
		*name = ft_substr(arg, 0, eq_pos - arg);
		*value = ft_strdup(eq_pos + 1);
	}
	else
	{
		*name = ft_strdup(arg);
		*value = NULL;
	}
	if (!is_valid_env_name(*name))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		free(*name);
		free(*value);
		return (1);
	}
	return (0);
}
