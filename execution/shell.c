#include "../includes/minishell.h"

static char	*find_path_in_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	return (env[i] + 5);
}

static char	*check_path_directory(char *path, char *cmd)
{
	char	*dir;
	char	*bin;

	dir = str_ndup(path, str_ichr(path, ':'));
	bin = join_path(dir, cmd);
	free(dir);
	if (access(bin, F_OK) == 0)
		return (bin);
	free(bin);
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*result;

	path = find_path_in_env(env);
	if (!path)
		return (cmd);
	while (path && str_ichr(path, ':') > -1)
	{
		result = check_path_directory(path, cmd);
		if (result)
			return (result);
		path += str_ichr(path, ':') + 1;
	}
	return (cmd);
}

int	shell_execute(t_ast *ast, char **env, int status)
{
	t_exec_data	data;

	data.exec = malloc(sizeof(t_execute));
	if (!data.exec || !ast || !env)
	{
		if (data.exec)
			free(data.exec);
		return (1);
	}
	data.exec->exit_status = status;
	data.exec->nb_pipes = 0;
	data.exec->pipfds = NULL;
	data.nb_pipes = count_pipes(ast);
	if (data.nb_pipes == 0)
		return (execute_single_command(&data, ast, env));
	data.ret = handle_pipes(ast, data.nb_pipes, data.exec, env);
	data.exit_status = data.exec->exit_status;
	free_exec(data.exec);
	return (data.exit_status);
}
