#include "../includes/minishell.h"

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

t_ast	*parser(const char *input)
{
	t_token	*tokens;
	t_token	*current;
	t_token	*print_token;
	t_ast	*ast;
	t_ast	*curr;
	char	*arg_copy;
	t_redir	*redir;
	t_ast	*new_node;

	tokens = tokenize(input);
	current = tokens;
	if (!tokens)
	{
		ft_putstr_fd("Error tokenizing input.\n", 2);
		return (NULL);
	}
	expand(g_data.env_list, tokens);
	print_token = tokens;
	while (print_token && print_token->type != TOKEN_EOF)
		print_token = print_token->next;
	ast = NULL;
	curr = create_ast_node();
	ast = curr;
	if (!curr)
	{
		ft_putstr_fd("Error creating AST node.\n", 2);
		return (NULL);
	}
	while (current)
	{
		if (current->type == TOKEN_EOF)
			break ;
		else if (current->type == TOKEN_WORD)
		{
			arg_copy = ft_strdup(current->value);
			ft_gc_add(arg_copy);
			if (!arg_copy)
			{
				return (NULL);
			}
			if (!ft_lst_push(&curr->args, arg_copy))
				return (NULL);
		}
		else if (ft_token_is_redirection(current->type))
		{
			if (current->next && current->next->type == TOKEN_WORD)
			{
				redir = ft_malloc(sizeof(t_redir));
				if (!redir)
					return (NULL);
				redir->type = current->type;
				redir->filename = ft_strdup(current->next->value);
				ft_gc_add(redir->filename);
				if (!redir->filename)
					return (NULL);
				if (!ft_lst_push(&curr->redirections, redir))
					return (NULL);
				current = current->next;
			}
			else
				return (NULL);
		}
		else if (current->type == TOKEN_PIPE)
		{
			if (current->next->type != TOKEN_WORD
				&& !ft_token_is_redirection(current->next->type))
			{
				ft_putstr_fd("Syntax error: Pipe not followed by a command or redirection.\n",
					2);
				return (NULL);
			}
			new_node = create_ast_node();
			if (!new_node)
			{
				ft_putstr_fd("Error creating new AST node for pipe.\n", 2);
				return (NULL);
			}
			add_ast_node(&ast, new_node);
			curr = new_node;
		}
		else
		{
			ft_putstr_fd("Unexpected token type: ", 2);
			ft_putnbr_fd(current->type, 2);
			ft_putstr_fd("\n", 2);
			return (NULL);
		}
		current = current->next;
	}
	free_tokens(tokens);
	return (ast);
}
