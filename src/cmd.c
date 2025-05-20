#include "../includes/minishell.h"

int ft_split_size(char **split) {
    int size = 0;
    while (split[size])
        size++;
    return size;
}

int get_len(char **s)
{
    if (!s || !*s)
        return (0);
    int i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

t_command *create_command(char *cmd) {
    int i = 0;
    t_command *new_node = malloc(sizeof(t_command));
    if (!new_node)
        return NULL;
    new_node->full_command = strdup(cmd);
    char **tokens = ft_split(cmd, ' ');
    // if (!tokens)
    // {}
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

void add_command(t_command **head, t_command *new_node) {
    if (!*head) {
        *head = new_node;
        return;
    }
    t_command *temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
}

void free_commands(t_command *head) {
    t_command *temp;
    while (head) {
        temp = head;
        free(head->command);
        free(head->arguments);
        free(head);
        head = temp->next;
    }
}
