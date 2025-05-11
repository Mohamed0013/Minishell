#include "../includes/minishell.h"

t_command *parse_input(char *input) {
    char        **tokens;
    t_command   *cmd;

    if (!input || !*input)
        return (NULL);
    tokens = ft_split(input, ' ');
    if (!tokens)
        return (NULL);
    cmd = create_command(tokens[0], tokens + 1);
    if (!cmd)
    {
        free_split(tokens);
        return (NULL);
    }
    free_split(tokens);
    return (cmd);
}