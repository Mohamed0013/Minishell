#include "minishell.h"

int	get_len(char **s)
{
	int	i;

	i = 0;
	if (!s || !*s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	execute_builtin_piped(t_execute *exec, char **cmd, t_list *redir)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return (-1);
	}
	else if (pid == 0)
	{
		if (redir && handle_redirections(redir) != 0)
			ft_exit_withclear(1);
		exec->exit_status = execute_builtin(cmd, redir, exec->exit_status);
		ft_exit_withclear(exec->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exec->exit_status = 128 + WTERMSIG(status);
	else if (WIFSTOPPED(status))
		exec->exit_status = 128 + WSTOPSIG(status);
	return (exec->exit_status);
}

int	execute_builtin_direct(t_execute *exec, char **cmd, t_list *redir)
{
	int	saved_fds[2];

	saved_fds[0] = -1;
	saved_fds[1] = -1;
	if (save_restore_fds(redir, saved_fds))
		return (1);
	exec->exit_status = execute_builtin(cmd, redir, exec->exit_status);
	restore_fds(redir, saved_fds);
	return (exec->exit_status);
}

int	execute_builtin(char **args, t_list *redirections, int exit_status)
{
	int	result;

	result = exec_builtin_part1(args, exit_status);
	if (result != -1)
		return (result);
	return (exec_builtin_part2(args, redirections));
}
