#include "../includes/minishell.h"

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

static void handle_pipes(t_command *cmd, char **env) {
    int pipefd[2];
    pid_t pid;
    char **commands = ft_split(cmd->arguments, '|');
    int num_commands = 0;
    
    // Count commands
    while (commands[num_commands]) num_commands++;

    int prev_pipe_in = 0;
    int i = 0;
    
    while (i < num_commands) {
        if (i < num_commands - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                free_split(commands);
                return;
            }
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            free_split(commands);
            return;
        }

        if (pid == 0) {  // Child process
            if (i > 0) {
                dup2(prev_pipe_in, STDIN_FILENO);
                close(prev_pipe_in);
            }
            
            if (i < num_commands - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]);
            }

            char **args = ft_split(commands[i], ' ');
            char *path = (str_ichr(args[0], '/') > -1) ? args[0] : get_path(args[0], env);
            
            execve(path, args, env);
            perror("execve");
            free_split(args);
            exit(EXIT_FAILURE);
        } else {  // Parent process
            if (i > 0) {
                close(prev_pipe_in);
            }
            
            if (i < num_commands - 1) {
                close(pipefd[1]);
                prev_pipe_in = pipefd[0];
            }
            
            i++;
        }
    }

    // Wait for all child processes
    for (i = 0; i < num_commands; i++) {
        wait(NULL);
    }

    free_split(commands);
}

void shell_loop(t_command *cmd, char **env) {
    if (!cmd) return;

    // Handle exit command
    if (strcmp(cmd->command, "exit") == 0) {
        printf("Exiting minishell...\n");
        free_commands(cmd);
        exit(0);
    }

    // Check for pipes in the command
    if (strchr(cmd->arguments, '|')) {
        handle_pipes(cmd, env);
        return;
    }

    // Regular command execution (non-piped)
    char **full_command = ft_split(cmd->arguments, ' ');
    if (!full_command) {
        perror("minishell");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free_split(full_command);
        return;
    }

    if (pid == 0) {  // Child process
        char *path;
        if (str_ichr(cmd->command, '/') > -1) {
            path = cmd->command;
        } else {
            path = get_path(cmd->command, env);
        }

        if (execve(path, full_command, env) == -1) {
            perror("minishell");
            free_split(full_command);
            exit(EXIT_FAILURE);
        }
    } else {  // Parent process
        int status;
        waitpid(pid, &status, 0);
        free_split(full_command);
    }
}
