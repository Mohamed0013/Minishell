#include "../includes/parsing.h"
#include "../includes/minishell.h"
#include "../includes/expansion.h"
#include "../includes/minishell2.h"

void	free_exec(t_execute *exec)
{
	if (!exec)
		return;
	if (exec->pipfds)
	{
		for (int i = 0; i < exec->nb_pipes; i++)
		{
			if (exec->pipfds[i])
			{
				close(exec->pipfds[i][0]);
				close(exec->pipfds[i][1]);
				free(exec->pipfds[i]);
			}
		}
		free(exec->pipfds);
	}
	free(exec);
}

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && str_ichr(path, ':') > -1)
	{
		dir = str_ndup(path, str_ichr(path, ':'));
		bin = join_path(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += str_ichr(path, ':') + 1;
	}
	return (cmd);
}

void	free_split(char **split)
{
	int i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int is_builtin(char *command)
{
	return (ft_strcmp(command, "cd") == 0 || ft_strcmp(command, "echo") == 0 ||
			ft_strcmp(command, "export") == 0 || ft_strcmp(command, "env") == 0 ||
			ft_strcmp(command, "exit") == 0 || ft_strcmp(command, "unset") == 0 ||
			ft_strcmp(command, "pwd") == 0);
}

int execute_builtin(char **args, t_list *redirections, char **env, int exit_status)
{
	if (ft_strcmp(args[0], "cd") == 0)
	{
		execute_cd(args);
		return 0;
	}
	else if (ft_strcmp(args[0], "echo") == 0)
	{
		execute_echo(args, exit_status);
		return 0;
	}
	else if (ft_strcmp(args[0], "export") == 0)
	{
		ft_export(&g_data.env_list, args);
		return 0;
	}
	else if (ft_strcmp(args[0], "env") == 0)
	{
		ft_env(g_data.env_list, args);
		return 0;
	}
	else if (ft_strcmp(args[0], "unset") == 0)
	{
		ft_unset(&g_data.env_list, args);
		return 0;
	}
	else if (ft_strcmp(args[0], "pwd") == 0)
	{
		execute_pwd();
		return 0;
	}
	else if (ft_strcmp(args[0], "exit") == 0)
	{
		// free_split(args);
		if (redirections)
			ft_lstclear(&redirections, free);
		return 2; // Special exit code
	}
	return -1; // Not a builtin
}

// Helper function to handle redirections
int handle_redirections(t_list *redirections)
{
    t_list *redir = redirections;
    
    while (redir)
    {
        t_redir *redir_info = (t_redir *)redir->content;
        
        if (redir_info->type == 1) // TOKEN_REDIRECT_IN
        {
            int fd = open(redir_info->filename, O_RDONLY);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(redir_info->filename, 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                return (1);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                close(fd);
                return (1);
            }
            close(fd);
        }
        else if (redir_info->type == 2 || redir_info->type == 3) // TOKEN_REDIRECT_OUT or TOKEN_APPEND
        {
            int flags = O_WRONLY | O_CREAT;
            flags |= (redir_info->type == 3) ? O_APPEND : O_TRUNC;
            
            int fd = open(redir_info->filename, flags, 0644);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(redir_info->filename, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                return (1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                close(fd);
                return (1);
            }
            close(fd);
        }
        redir = redir->next;
    }
    return (0);
}

int	execute_command(t_execute *exec, char **full_command, t_list *redirections, char **env, int is_piped)
{
	char	*path;
	int		status;
	pid_t	pid;

	if (is_builtin(full_command[0]))
	{
		// For builtins: only fork if we're in a pipeline
		if (is_piped)
		{
			pid = fork();
			if (pid < 0)
			{
				perror("Fork failed");
				return (-1);
			}
			else if (pid == 0) // Child process
			{
				// Handle redirections in child
				if (redirections && handle_redirections(redirections) != 0)
				{
					free_split(full_command);
					exit(1);
				}
				
				exec->exit_status = execute_builtin(full_command, redirections, env, exec->exit_status);
				free_split(full_command);
				exit(exec->exit_status);
			}
			else // Parent process
			{
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					exec->exit_status = WEXITSTATUS(status);
			}
		}
		else
		{
			// Execute builtin directly in parent (no fork)
			// Handle redirections if any (save/restore file descriptors)
			int saved_stdin = -1, saved_stdout = -1;
			if (redirections)
			{
				saved_stdin = dup(STDIN_FILENO);
				saved_stdout = dup(STDOUT_FILENO);
				if (handle_redirections(redirections) != 0)
				{
					if (saved_stdin != -1) close(saved_stdin);
					if (saved_stdout != -1) close(saved_stdout);
					return (1);
				}
			}
			
			exec->exit_status = execute_builtin(full_command, redirections, env, exec->exit_status);
			
			// Restore original file descriptors
			if (redirections)
			{
				if (saved_stdin != -1)
				{
					dup2(saved_stdin, STDIN_FILENO);
					close(saved_stdin);
				}
				if (saved_stdout != -1)
				{
					dup2(saved_stdout, STDOUT_FILENO);
					close(saved_stdout);
				}
			}
		}
		
		if (exec->exit_status == 2) // exit command
			return (2);
	}
	else
	{
		// External commands: always fork
		pid = fork();
		if (pid < 0)
		{
			perror("Fork failed");
			return (-1);
		}
		else if (pid == 0) // Child process
		{
			// Handle redirections
			if (redirections && handle_redirections(redirections) != 0)
			{
				free_split(full_command);
				exit(1);
			}

			// Find executable path
			if (str_ichr(full_command[0], '/') > -1)
				path = full_command[0];
			else
				path = get_path(full_command[0], env);
				
			if (execve(path, full_command, env) == -1)
			{
				if (errno == ENOENT)
				{
					printf("minishell: %s: command not found\n", full_command[0]);
					exec->exit_status = 127;
				}
				else if (errno == EACCES)
				{
					printf("minishell: %s: Permission denied\n", full_command[0]);
					exec->exit_status = 126;
				}
				else
				{
					perror("minishell");
					exec->exit_status = 1;
				}
				free_split(full_command);
				exit(exec->exit_status);
			}
		}
		else // Parent process
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				exec->exit_status = WEXITSTATUS(status);
		}
	}
	
	return (exec->exit_status);
}

char	**get_args(t_list *args)
{
	char	**result;
	int		i;
	t_list	*current;

	if (!args)
		return (NULL);
	result = malloc(sizeof(char *) * (ft_lstsize(args) + 1));
	if (!result)
		return (NULL);
	i = 0;
	current = args;
	while (current)
	{
		result[i] = ft_strdup((char *)current->content);
		if (!result[i])
		{
			// Free previously allocated strings if strdup fails
			while (--i >= 0)
				free(result[i]);
			free(result);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	result[i] = NULL; // Null-terminate the array
	return (result);
}

char	*ft_strconcat(char **args, const char *sep)
{
	char	*result;
	int		i;
	int		total_length;

	if (!args || !*args)
		return (NULL);

	total_length = 0;
	for (i = 0; args[i]; i++)
		total_length += ft_strlen(args[i]) + (sep ? ft_strlen(sep) : 0);
	if (total_length == 0)
		return (NULL);

	result = malloc(total_length + 1);
	if (!result)
		return (NULL);

	result[0] = '\0'; // Initialize the result string
	for (i = 0; args[i]; i++)
	{
		if (i > 0 && sep)
			ft_strlcat(result, sep, total_length + 1);
		ft_strlcat(result, args[i], total_length + 1);
	}
	return (result);
}

int count_pipes(t_ast *ast)
{
	int count = 0;
	t_ast *current = ast;

	while (current)
	{
		if (current->next)
			count++;
		current = current->next;
	}
	return count;
}

void	fill_pipes(int **pipfds, int nb_pipes)
{
	for (int i = 0; i < nb_pipes; i++)
	{
		pipfds[i] = malloc(2 * sizeof(int));
		if (!pipfds[i]) {
			// Free previously allocated pipes
			for (int j = 0; j < i; j++) {
				close(pipfds[j][0]);
				close(pipfds[j][1]);
				free(pipfds[j]);
			}
			perror("malloc failed for pipe");
			exit(EXIT_FAILURE);
		}
		if (pipe(pipfds[i]) == -1) {
			// Free current pipe and all previously allocated pipes
			free(pipfds[i]);
			for (int j = 0; j < i; j++) {
				close(pipfds[j][0]);
				close(pipfds[j][1]);
				free(pipfds[j]);
			}
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}
	}
}

int	set_pipefds(int nb_pipes, t_execute *exec)
{
	exec->nb_pipes = nb_pipes;
	if (nb_pipes > 0)
	{
		exec->pipfds = malloc((nb_pipes + 1) * sizeof(int *));
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

int handle_pipes(t_ast *ast, int nb_pipes, t_execute *exec, char **env)
{
	t_ast	*current;
	char	**first_cmd;
	int		i;
	pid_t	*pids;

	i = 0;
	current = ast;
	if (set_pipefds(nb_pipes, exec))
		return (1);
	
	// Check for exit command in pipeline
	first_cmd = get_args(current->args);
	if (first_cmd && strcmp(first_cmd[0], "exit") == 0)
	{
		int exit_status = exec->exit_status;
		free_split(first_cmd);
		return (2); // Signal exit to caller
	}
	free_split(first_cmd);
		
	pids = malloc((nb_pipes + 1) * sizeof(pid_t));
	if (!pids)
		return (1);

	while (current)
	{
		char **full_command = get_args(current->args);
		if (!full_command)
		{
			free(pids);
			return (1);
		}

		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("Fork failed");
			free_split(full_command);
			free(pids);
			return (1);
		}
		else if (pids[i] == 0)  // Child process
		{
			// Check if this command has output redirection
			int has_output_redirect = 0;
			int has_input_redirect = 0;
			if (current->redirections)
			{
				t_list *redir = current->redirections;
				while (redir)
				{
					t_token *token = (t_token *)redir->content;
					if (token->type == TOKEN_REDIRECT_OUT || token->type == TOKEN_APPEND)
						has_output_redirect = 1;
					if (token->type == TOKEN_REDIRECT_IN)
						has_input_redirect = 1;
					redir = redir->next;
				}
			}

			// Set up pipes first (only if no conflicting redirections)
			if (nb_pipes > 0) {
				// Set up pipe for stdout only if no output redirection
				if (i < nb_pipes && !has_output_redirect)
					dup2(exec->pipfds[i][1], STDOUT_FILENO);
				
				// Set up pipe for stdin only if no input redirection
				if (i > 0 && !has_input_redirect)
					dup2(exec->pipfds[i - 1][0], STDIN_FILENO);

				// Close all pipes in child
				for (int j = 0; j < nb_pipes; j++)
				{
					if (exec->pipfds[j]) {
						close(exec->pipfds[j][0]);
						close(exec->pipfds[j][1]);
					}
				}
			}

			// Handle redirections (these will take precedence over pipes)
			if (current->redirections && handle_redirections(current->redirections) != 0)
				exit(1);

			// Execute the command
			if (is_builtin(full_command[0]))
			{
				int builtin_status = execute_builtin(full_command, current->redirections, env, exec->exit_status);
				exit(builtin_status == 2 ? exec->exit_status : builtin_status);
			}
			else
			{
				// External command
				char *path;
				if (str_ichr(full_command[0], '/') > -1)
					path = full_command[0];
				else
					path = get_path(full_command[0], env);
					
				if (execve(path, full_command, env) == -1)
				{
					if (errno == ENOENT)
					{
						printf("minishell: %s: command not found\n", full_command[0]);
						exit(127);
					}
					else if (errno == EACCES)
					{
						printf("minishell: %s: Permission denied\n", full_command[0]);
						exit(126);
					}
					else
					{
						perror("minishell");
						exit(1);
					}
				}
			}
		}

		// Parent process - close pipe ends
		if (nb_pipes > 0) {
			if (i > 0 && exec->pipfds[i - 1])
				close(exec->pipfds[i - 1][0]);
			if (i < nb_pipes && exec->pipfds[i])
				close(exec->pipfds[i][1]);
		}

		free_split(full_command);
		current = current->next;
		i++;
	}

	// Wait for all processes
	int last_status = 0;
	for (int j = 0; j < i; j++)
	{
		int status;
		waitpid(pids[j], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
	}

	free(pids);
	exec->exit_status = last_status;
	return (exec->exit_status);
}

int shell_execute(t_ast *ast, char **env, int status)
{
	t_execute *exec = malloc(sizeof(t_execute));
	if (!exec)
		return (1);
		
	exec->exit_status = status;
	exec->nb_pipes = 0;
	exec->pipfds = NULL;
	if (!ast || !env)
	{
		free(exec);
		return (1);
	}

	int nb_pipes = count_pipes(ast);
	
	// If no pipes, execute single command
	if (nb_pipes == 0)
	{
		char **full_command = get_args(ast->args);
		if (!full_command)
		{
			free(exec);
			return (1);
		}
		
		int ret = execute_command(exec, full_command, ast->redirections, env, 0); // is_piped = 0
		free_split(full_command);
		int exit_status = exec->exit_status;
		free(exec);
		return (ret == 2 ? 2 : exit_status); // Return 2 for exit command
	}
	else
	{
		// Handle pipeline
		int ret = handle_pipes(ast, nb_pipes, exec, env);
		int exit_status = exec->exit_status;
		free_exec(exec);
		return (exit_status);
	}
}