#include "../includes/minishell.h"

void handle_pipes(char *input, char **env)
{
    char **commands;
    int (num_cmds) = 0;
    int (success) = 1;
    int (i) = 0;
    int (j) = 0;
    int (*pipes)[2] = NULL;
    pid_t *pids = NULL;

    commands = split_commands(input);
    if (!commands) return;
    // Count commands and validate
    while (commands[num_cmds]) num_cmds++;
    if (num_cmds < 2) {
        free_split(commands);
        return ;
    }
    // Allocate pipes and pids
    pipes = malloc((num_cmds - 1) * sizeof(int[2]));
    pids = malloc(num_cmds * sizeof(pid_t));
    if (!pipes || !pids) {
        perror("malloc");
        free(pipes);
        free(pids);
        free_split(commands);
        return;
    }
    // Initialize pipes and processes
    while (i < num_cmds)
    {
        if (i < num_cmds - 1 && pipe(pipes[i]) == -1) {
            perror("pipe");
            success = 0;
            break ;
        }
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            success = 0;
            break ;
        }
        if (pids[i] == 0) { // Child process
            // Close all pipes except those we need
            while (j < num_cmds - 1)
            {
                if (j != i - 1)
                    close(pipes[j][0]);
                if (j != i)
                    close(pipes[j][1]);
                j++;
            }
            // Handle input redirection
            if (i > 0)
            {
                dup2(pipes[i - 1][0], 0);
                close(pipes[i - 1][0]);
            }

            // Handle output redirection
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], 1);
                close(pipes[i][1]);
            }

            // Execute command
            t_command *cmd = parse_command(commands[i]);
            if (cmd) {
                execute_single_command(cmd, env);
                free_command(cmd);
            }
            exit(EXIT_SUCCESS);
        }
        i++;
    }

    // Parent cleanup
    if (success) {
        // Close all pipes in parent
        for (int j = 0; j < num_cmds-1; j++) {
            close(pipes[j][0]);
            close(pipes[j][1]);
        }

        // Wait for all children
        for (int i = 0; i < num_cmds; i++) {
            waitpid(pids[i], NULL, 0);
        }
    } else {
        // Cleanup on failure
        for (int j = 0; j < num_cmds-1; j++) {
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
        for (int i = 0; i < num_cmds; i++) {
            if (pids[i] > 0) kill(pids[i], SIGTERM);
        }
    }

    free(pipes);
    free(pids);
    free_split(commands);
}