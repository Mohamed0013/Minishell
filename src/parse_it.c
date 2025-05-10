#include "../includes/minishell.h"

t_command *parse_input(char *input) {
    char        **tokens;
    t_command   *cmd;
    char        **args;
    int         i, arg_count;

    if (!input || !*input)
        return (NULL);

    tokens = ft_split(input, ' ');
    if (!tokens) {
        free(input);
        return (NULL);
    }

    // Count the number of arguments
    for (arg_count = 0; tokens[arg_count]; arg_count++);

    // Duplicate the arguments array
    args = malloc((arg_count) * sizeof(char *));
    if (!args) {
        free_split(tokens);
        free(input);
        return (NULL);
    }
    for (i = 1; i < arg_count; i++) {
        args[i - 1] = strdup(tokens[i]);
        if (!args[i - 1]) {
            while (--i >= 1)
                free(args[i - 1]);
            free(args);
            free_split(tokens);
            free(input);
            return (NULL);
        }
    }
    args[arg_count - 1] = NULL;

    cmd = create_command(tokens[0], args);
    if (!cmd) {
        free_split(tokens);
        free_split(args);
        free(input);
        return (NULL);
    }

    free_split(tokens);
    free(input);
    return (cmd);
}