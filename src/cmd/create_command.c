#include "../includes/minishell.h"

t_command *create_command(char *cmd) {
    int i = 0;
    t_command *new_node = malloc(sizeof(t_command));
    if (!new_node)
        return NULL;
    new_node->full_command = strdup(cmd);
    char **tokens = ft_split(cmd, ' ');
    if (!tokens)
    {
        free(new_node);
        return (NULL);
    }
    int len = get_len(tokens);
    new_node->command = strdup(tokens[0]);
    new_node->arguments = NULL;
    if (len > 1)
    {
        new_node->arguments = malloc(sizeof(char *) * (ft_split_size(tokens) + 1));
        if (!new_node->arguments) {
            free(new_node->full_command);
            free(new_node);
            free_split(tokens);
            return NULL;
        }
        while (tokens[i]) {
            new_node->arguments[i] = strdup(tokens[i]);
            i++;
        }
        new_node->arguments[i] = NULL;
    }
    new_node->next = NULL;
    free_split(tokens);
    return (new_node);
}