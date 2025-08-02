# include "minishell.h"

t_list ** gc_ptr()
{
    static t_list *head = NULL;
    return &head;
}

void * ft_malloc(size_t size)
{
    t_list **head = gc_ptr();
    t_list *new_node = ft_lstnew(NULL);
    if (!new_node)
        return NULL;
    new_node->content = malloc(size);
    if (!new_node->content)
    {
        free(new_node);
        return NULL;
    }
    new_node->next = *head;
    *head = new_node;
    return new_node->content;
}
void ft_gc_clear()
{
    t_list *current = *gc_ptr();
    ft_lstclear(&current, free);
    *gc_ptr() = NULL;
}

void ft_gc_add(void *ptr)
{
    t_list **head = gc_ptr();
    t_list *new_node = ft_lstnew(ptr);
    if (!new_node)
        return;
    new_node->next = *head;
    *head = new_node;
}