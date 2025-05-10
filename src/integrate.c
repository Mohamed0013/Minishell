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


void execute_command(t_command *cmd, char **env) {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        if (execvp(cmd->command, cmd->arguments) == -1) {
            perror("minishell");
        }
        exit(EXIT_FAILURE); // Exit if execvp fails
    } else if (pid < 0) { // Fork failed
        perror("minishell");
    } else { // Parent process
        int status;
        // waitpid(pid, &status, 0); // Wait for the child process to finish
    }
}

void free_command(t_command *cmd) {
    if (cmd) {
        free(cmd->command); // Free command string
        free_split(cmd->arguments); // Free arguments array
        free(cmd); // Free command structure
    }
}

void shell_loop(t_command *cmd, char **env) {
    char *input;
    t_command *parsed_cmd;

    while (1) {
        input = readline("minishell> "); // Prompt for input
        if (!input) { // Check for EOF
            printf("\n");
            break;
        }
        remove_newline(input); // Remove newline character
        if (strlen(input) == 0) { // Ignore empty input
            free(input);
            continue;
        }
        add_history(input); // Add input to history
        parsed_cmd = parse_input(input); // Parse the input
        if (parsed_cmd) {
            execute_command(parsed_cmd, env); // Execute the parsed command
            free_command(parsed_cmd); // Free the command structure
        }
        free(input); // Free the input string
    }
}