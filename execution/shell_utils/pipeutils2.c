#include "../../includes/minishell.h"

int	set_pipefds(int nb_pipes, t_execute *exec)
{
	exec->nb_pipes = nb_pipes;
	if (nb_pipes > 0)
	{
		exec->pipfds = ft_malloc((nb_pipes + 1) * sizeof(int *));
		if (!exec->pipfds)
			return (1);
		exec->pipfds[nb_pipes] = NULL;
		fill_pipes(exec->pipfds, nb_pipes);
	}
	else
	{
		exec->pipfds = NULL;
	}
	return (0);
}

void	setup_child_pipes(t_execute *exec, int i, int nb_pipes, t_list *redir)
{
	int	has_output;
	int	has_input;
	int	j;

	check_redirect_types(redir, &has_output, &has_input);
	if (nb_pipes > 0)
	{
		if (i < nb_pipes && !has_output)
			dup2(exec->pipfds[i][1], STDOUT_FILENO);
		if (i > 0 && !has_input)
			dup2(exec->pipfds[i - 1][0], STDIN_FILENO);
		j = 0;
		while (j < nb_pipes)
		{
			if (exec->pipfds[j])
			{
				close(exec->pipfds[j][0]);
				close(exec->pipfds[j][1]);
			}
			j++;
		}
	}
}

void	close_parent_pipes(t_execute *exec, int i, int nb_pipes)
{
	if (nb_pipes > 0)
	{
		if (i > 0 && exec->pipfds[i - 1])
			close(exec->pipfds[i - 1][0]);
		if (i < nb_pipes && exec->pipfds[i])
			close(exec->pipfds[i][1]);
	}
}

void	execute_child_builtin(t_child_data *data, t_ast *current,
		t_execute *exec)
{
	data->status = execute_builtin(data->full_command, current->redirections,
			exec->exit_status);
	if (data->status == 2)
		ft_exit_withclear(exec->exit_status);
	else
		ft_exit_withclear(data->status);
}

void	execute_child_external_pipe(t_child_data *data, char **env)
{
	if (str_ichr(data->full_command[0], '/') > -1)
		data->path = data->full_command[0];
	else
		data->path = get_path(data->full_command[0], env);
	if (execve(data->path, data->full_command, env) == -1)
	{
		if (errno == ENOENT)
			ft_exit_withclear(127);
		else if (errno == EACCES)
			ft_exit_withclear(126);
		else
			ft_exit_withclear(1);
	}
}
