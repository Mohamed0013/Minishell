#include "minishell.h"

t_ast	*create_ast_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->redirections = NULL;
	node->next = NULL;
	return (node);
}

void	add_ast_node(t_ast **head, t_ast *new_node)
{
	t_ast	*current;

	if (!head || !new_node)
		return ;
	if (!*head)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

bool	ft_token_is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	ft_lst_push(t_list **head, void *value)
{
	t_list	*new_node;

	new_node = ft_lstnew(value);
	ft_gc_add(new_node);
	if (!new_node)
		return (0);
	ft_lstadd_back(head, new_node);
	return (1);
}

void	free_ast(t_ast *ast)
{
	t_list	*curr;
	t_redir	*redir;

	if (!ast)
		return ;
	if (ast->args)
		ft_lstclear(&ast->args, free);
	if (ast->redirections)
	{
		curr = ast->redirections;
		while (curr)
		{
			redir = (t_redir *)curr->content;
			if (redir)
			{
				if (redir->filename)
					free(redir->filename);
				free(redir);
			}
			curr = curr->next;
		}
		ft_lstclear(&ast->redirections, NULL);
	}
}
