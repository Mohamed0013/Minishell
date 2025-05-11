#include "../includes/minishell.h"

static void remove_newline(char *line) {
    size_t i = 0;

    while (line[i]) { // Iterate through the string
        if (line[i] == '\n') { // Check for newline character
            line[i] = '\0';    // Replace it with null terminator
            break;             // Stop once newline is replaced
        }
        i++;
    }
}

void shell_loop(t_command *cmd, char **env) {
    char **full_command = ft_split(cmd->arguments, ' ');
    if (!cmd) {
        return; // If the command is NULL, exit the function
    }

    // Exit condition
    if (strcmp(full_command[0], "exit") == 0) {
        printf("Exiting minishell...\n");
        free_commands(cmd); // Free the command structure
        exit(0); // Exit the shell
    }

    pid_t pid = fork(); // Create a child process
    if (pid < 0) {
        perror("fork"); // Handle fork error
        return;
    }
    if (pid == 0) {
        // Child process: Execute the command
        if (execve(cmd->command, full_command, env) == -1) {
            perror("minishell"); // Print error if execve fails
        }
        exit(EXIT_FAILURE); // Exit child process if execve fails
    } else {
        // Parent process: Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);
    }
}
