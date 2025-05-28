#include "../includes/minishell.h"

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
