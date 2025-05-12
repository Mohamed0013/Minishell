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

void shell_loop(t_command *cmd, char **env) {
    if (!cmd) {
        return;
    }

    // Check if there is a pipe in the command
    t_command *current = cmd;
    int has_pipe = 0;
    while (current) {
        if (strchr(current->arguments, '|')) {
            has_pipe = 1;
            break;
        }
        current = current->next;
    }
    if (!has_pipe) {
        // No pipe: Execute the command normally
        char *path;
        char **full_command = ft_split(cmd->arguments, ' ');
        if (strcmp(full_command[0], "exit") == 0) {
            printf("Exiting minishell...\n");
            free_commands(cmd);
            exit(0);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return;
        }
        if (str_ichr(cmd->command, '/') > -1)
            path = cmd->command;
        else
            path = get_path(cmd->command, env);
        if (pid == 0) {
            if (execve(path, full_command, env) == -1) {
                perror("minishell");
            }
            exit(EXIT_FAILURE);
        } else {
            // Parent process: Wait for the child process to finish
            int status;
            waitpid(pid, &status, 0);
        }
    } else {
        // Handle commands with pipes
        int pipe_fd[2];
        int prev_fd = -1; // To store the read end of the previous pipe
        pid_t pid;

        while (cmd) {
            if (cmd->next) {
                if (pipe(pipe_fd) == -1) {
                    perror("pipe");
                    return;
                }
            }

            pid = fork();
            if (pid == -1) {
                perror("fork");
                return;
            }

            if (pid == 0) { // Child process
                if (prev_fd != -1) {
                    dup2(prev_fd, STDIN_FILENO); // Read from the previous pipe
                    close(prev_fd);
                }
                if (cmd->next) {
                    dup2(pipe_fd[1], STDOUT_FILENO); // Write to the current pipe
                    close(pipe_fd[1]);
                    close(pipe_fd[0]);
                }
                char **full_command = ft_split(cmd->arguments, ' ');
                execve(get_path(cmd->command, env), full_command, env);
                perror("execve");
                exit(EXIT_FAILURE);
            } else { // Parent process
                if (prev_fd != -1)
                    close(prev_fd);
                if (cmd->next) {
                    close(pipe_fd[1]);
                    prev_fd = pipe_fd[0];
                }
                waitpid(pid, NULL, 0);
            }
            cmd = cmd->next;
        }
    }
}
