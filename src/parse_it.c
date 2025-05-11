#include "../includes/minishell.h"

t_command *parse_input(char *input) {
    // char        **tokens;
    t_command   *cmd;

    if (!input || !*input)
        return (NULL);
    // tokens = ft_split(input, ' ');
    // if (!tokens)
    //     return (NULL);
    cmd = create_command(input);
    if (!cmd) {
        // free_split(tokens);
        return (NULL);
    }
    // free_split(tokens);
    if (!cmd)
        return (NULL);
    return (cmd);
}