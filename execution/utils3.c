/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:30 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:30 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	execute_child_external(t_external_data *data, char **cmd,
		t_list *redir, char **env)
{
	int	err;

	if (redir && handle_redirections(redir) != 0)
		ft_exit_withclear(1);
	if (str_ichr(cmd[0], '/') > -1)
		data->path = cmd[0];
	else
		data->path = get_path(cmd[0], env);
	if (execve(data->path, cmd, env) == -1)
	{
		err = errno;
		exec_error(err, cmd);
	}
}

int	condition(t_external_data *data, char **cmd, t_list *redir, char **env)
{
	if (data->pid < 0)
	{
		perror("Fork failed");
		return (-1);
	}
	else if (data->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		execute_child_external(data, cmd, redir, env);
		ft_exit_withclear(0);
	}
	return (0);
}

static int	execute_external_cmd(t_execute *exec, char **cmd, t_list *redir,
		char **env)
{
	t_external_data	data;

	data.pid = fork();
	if (condition(&data, cmd, redir, env) == -1)
		return (-1);
	waitpid(data.pid, &data.status, 0);
	if (WIFEXITED(data.status))
		exec->exit_status = WEXITSTATUS(data.status);
	else if (WIFSIGNALED(data.status))
	{
		exec->exit_status = 128 + WTERMSIG(data.status);
		if (exec->exit_status == 128 + SIGINT || exec->exit_status == 128
			+ SIGQUIT)
			write(1, "\n", 1);
	}
	else if (WIFSTOPPED(data.status))
		exec->exit_status = 128 + WSTOPSIG(data.status);
	return (exec->exit_status);
}

int	execute_command(t_execute *exec, char **cmd, t_list *redir, char **env)
{
	if (is_builtin(cmd[0]))
	{
		if (exec->nb_pipes > 0)
			return (execute_builtin_piped(exec, cmd, redir));
		else
			return (execute_builtin_direct(exec, cmd, redir));
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
		if (*name)
			free(*name);
		if (*value)
			free(*value);
		return (1);
	}
	return (0);
}
