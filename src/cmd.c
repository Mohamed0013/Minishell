#include "../includes/minishell.h"

t_command *create_command(char *cmd) {
    t_command *new_node = malloc(sizeof(t_command));
    if (!new_node)
        return NULL;
    new_node->arguments = strdup(cmd);
    char **tokens = ft_split(cmd, ' ');
    new_node->command = strdup(tokens[0]);
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
        // free(head->arguments);
        free(head);
        head = temp->next;
    }
}
