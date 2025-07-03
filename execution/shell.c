#include "../includes/parsing.h"
#include "../includes/minishell.h"
#include "../includes/expansion.h"
#include "../includes/minishell2.h"

void    free_exec(t_execute *exec)
{
    if (!exec)
        return;
    if (exec->pipfds)
    {
        for (int i = 0; exec->pipfds[i]; i++)
        {
            free(exec->pipfds[i]);
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

void free_split(char **split)
{
    int i;

    if (!split)
        return;
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
            ft_strcmp(command, "exit") == 0);
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
    else if (ft_strcmp(args[0], "exit") == 0)
    {
        free_split(args);
        if (redirections)
            ft_lstclear(&redirections, free);
        return 2; // Special exit code
    }
    return -1; // Not a builtin
}

int	execute_command(t_execute *exec, char **full_command, t_list *redirections, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;
    int     status;
	pid_t   pid;

	i = 0;
	if (is_builtin(full_command[0]))
    {
        exec->exit_status = execute_builtin(full_command, redirections, env, exec->exit_status);
        if (exec->exit_status == 2)
            return (2);
    }
    else
    {
        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            exit (-1);
        }
        else if (pid == 0) // Child process
        {
            // Set up redirections if any
            if (redirections)
            {
                t_list *redir = redirections;
                while (redir)
                {
                    t_token *token = (t_token *)redir->content;
                    if (token->type == TOKEN_REDIRECT_IN)
                    {
                        int fd = open(token->value, O_RDONLY);
                        if (fd < 0)
                        {
                            perror("Open input redirection failed");
                            free_split(full_command);
                            exec->exit_status = 1; // Error opening file
                            exit(exec->exit_status);
                        }
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                    }
                    else if (token->type == TOKEN_REDIRECT_OUT || token->type == TOKEN_APPEND)
                    {
                        int flags = O_WRONLY | O_CREAT | (token->type == TOKEN_APPEND ? O_APPEND : O_TRUNC);
                        int fd = open(token->value, flags, 0644);
                        if (fd < 0)
                        {
                            perror("Open output redirection failed");
                            free_split(full_command);
                            exec->exit_status = 1; // Error opening file
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                    redir = redir->next;
                }
            }

            if (str_ichr(full_command[0], '/') > -1)
                path = full_command[0];
            else
                path = get_path(full_command[0], env);
            if (execve(path, full_command, env) == -1)
            {
                perror("minishell");
                free_split(full_command);
                exec->exit_status = 127; // Command not found
                exit(exec->exit_status);
            }
        }
        else {  // Parent process
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)){
                status = WEXITSTATUS(status);
                exec->exit_status = status;
            }
            free_split(full_command);
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
		result[i++] = ft_strdup((char *)current->content);
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

void    fill_pipes(int **pipfds, int nb_pipes)
{
    for (int i = 0; i < nb_pipes; i++)
    {
        pipfds[i] = malloc(2 * sizeof(int));
        if (pipe(pipfds[i]) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }
}


int handle_pipes(t_ast *ast, int nb_pipes, t_execute *exec, char **env)
{
    t_ast *current = ast;
    int i = 0;
    
    // Initialize pipes only if needed
    if (nb_pipes > 0) {
        exec->pipfds = malloc((nb_pipes + 1) * sizeof(int *));
        if (!exec->pipfds)
            return (1);
        exec->pipfds[nb_pipes] = NULL;
        fill_pipes(exec->pipfds, nb_pipes);
    } else
        exec->pipfds = NULL;
    char **first_cmd = get_args(current->args);
    if (first_cmd && strcmp(first_cmd[0], "exit") == 0)
    {
        int exit_status = exec->exit_status;
        free_split(first_cmd);
        free_exec(exec);
        free_ast(ast);
        exit(exit_status);
    }
    free_split(first_cmd);
    
    pid_t *pids = malloc((nb_pipes + 1) * sizeof(pid_t));
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
            if (nb_pipes > 0) {
                // Set up pipes for current command
                if (i < nb_pipes)
                    dup2(exec->pipfds[i][1], STDOUT_FILENO);
                if (i > 0)
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

            execute_command(exec, full_command, current->redirections, env);
            exit(exec->exit_status);
        }

        // Parent process
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
    if (!ast || !env)
    {
        free(exec);
        return (1);
    }

    int nb_pipes = count_pipes(ast);
    int ret = handle_pipes(ast, nb_pipes, exec, env);
    
    // Cleanup AST after execution
    free_ast(ast);
    free_exec(exec);
    return (ret);
}
