#include "../includes/minishell.h"

void execute_cd(char **arguments) {
    char *current_dir;
    char *new_dir;
    struct stat parent_stat;
    if (get_len(arguments) == 0) {
        // No argument provided, default to HOME
        char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            return ;
        }
        if (chdir(home) == -1) {
            perror("minishell: cd");
        }
    }
    else if (strcmp(arguments[1], "..") == 0) {
        // Check if the parent directory exists
        if (stat("..", &parent_stat) == -1) {
            // Parent directory does not exist
            printf("minishell: cd: error retrieving parent directory: %s\n", strerror(errno));
            return;
        }

        // Handle `cd ..` case
        current_dir = getcwd(NULL, 0);
        if (!current_dir) {
            printf("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
            return;
        }

        if (chdir("..") == -1) {
            perror("minishell: cd");
            free(current_dir);
            return;
        }

        // Check if the new directory is valid
        new_dir = getcwd(NULL, 0);
        if (!new_dir) {
            printf("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");

            // Manually update PWD to reflect the logical path
            char *last_slash = strrchr(current_dir, '/');
            if (last_slash) {
                *last_slash = '\0'; // Remove the last component of the path
                setenv("PWD", current_dir, 1); // Update PWD manually
            }
            free(current_dir);
            return;
        }

        setenv("PWD", new_dir, 1); // Update PWD environment variable
        free(current_dir);
        free(new_dir);
    }
    else {
        // Change to the specified directory
        if (chdir(arguments[1]) == -1) {
            fprintf(stderr, "minishell: cd: %s: %s\n", arguments[1], strerror(errno));
        }
    }
}