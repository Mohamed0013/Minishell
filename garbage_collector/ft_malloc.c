/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:42 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:42 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	**gc_ptr(void)
{
	static t_list	*head;

	return (&head);
}

void	*ft_malloc(size_t size)
{
	t_list	**head;
	t_list	*new_node;

	head = gc_ptr();
	new_node = ft_lstnew(NULL);
	if (!new_node)
		return (NULL);
	new_node->content = malloc(size);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = *head;
	*head = new_node;
	return (new_node->content);
}

void	ft_gc_clear(void)
{
	t_list	*current;

	current = *gc_ptr();
	ft_lstclear(&current, free);
	*gc_ptr() = NULL;
}

void	ft_gc_add(void *ptr)
{
	t_list	**head;
	t_list	*new_node;

	if (!ptr)
		return ;
	head = gc_ptr();
	new_node = ft_lstnew(ptr);
	if (!new_node)
		return ;
	new_node->next = *head;
	*head = new_node;
}
