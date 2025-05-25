#include "../includes/minishell.h"

void handle_pipes(t_command *cmd, char **env)
{
    int pipefd[2];
    pid_t pid;
    char **commands = ft_split(cmd->full_command, '|');
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