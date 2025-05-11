#include "../includes/minishell.h"

t_command *create_command(char *cmd, char **args) {
    t_command *new_node = malloc(sizeof(t_command));
    if (!new_node)
        return NULL;
    new_node->command = strdup(cmd);
    int i = 0;
    new_node->arguments = malloc(sizeof(char *) * (sizeof(args) + 1));
    if (!new_node->command || !new_node->arguments) {
        free(new_node->command);
        free(new_node->arguments);
        free(new_node);
        return NULL;
    }
    while (*args) {
        new_node->arguments[i] = malloc(sizeof(char) * (strlen(*args) + 1));
        if (!new_node->arguments[i]) {
            free(new_node->command);
            free(new_node->arguments);
            free(new_node);
            return NULL;
        }
        strcpy(new_node->arguments[i], *args);
        new_node->arguments[i] = strdup(*args);
        if (!new_node->arguments[0]) {
            free(new_node->command);
            free(new_node->arguments);
            free(new_node);
            return NULL;
        }
        i++;
        args++;
    }
    new_node->arguments[i] = NULL; // Null-terminate the arguments array
    new_node->next = NULL;
    return new_node;
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
