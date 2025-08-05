#include "minishell.h"

static int	handle_word_token(t_ast *curr, t_token *current)
{
	char	*arg_copy;

	arg_copy = ft_strdup(current->value);
	ft_gc_add(arg_copy);
	if (!arg_copy)
		return (0);
	if (!ft_lst_push(&curr->args, arg_copy))
		return (0);
	return (1);
}

static int	handle_redirection_token(t_ast *curr, t_token *current)
{
	t_redir	*redir;

	if (current->next && current->next->type == TOKEN_WORD)
	{
		redir = ft_malloc(sizeof(t_redir));
		if (!redir)
			return (0);
		redir->type = current->type;
		redir->filename = ft_strdup(current->next->value);
		ft_gc_add(redir->filename);
		if (!redir->filename)
			return (0);
		if (!ft_lst_push(&curr->redirections, redir))
			return (0);
		return (2);
	}
	return (0);
}

static int	handle_pipe_token(t_ast **ast, t_ast **curr, t_token *current)
{
	t_ast	*new_node;

	if (current->next->type != TOKEN_WORD
		&& !ft_token_is_redirection(current->next->type))
	{
		ft_putstr_fd("Syntax error: Pipe not followed by a command or\
			redirection.\n", 2);
		return (0);
	}
	new_node = create_ast_node();
	if (!new_node)
	{
		ft_putstr_fd("Error creating new AST node for pipe.\n", 2);
		return (0);
	}
	add_ast_node(ast, new_node);
	*curr = new_node;
	return (1);
}

t_ast	*parser(const char *input)
{
	t_token	(*tokens), (*current);
	t_ast	(*ast), (*curr);
	int		ret;

	tokens = tokenize(input);
	current = tokens;
	if (!tokens)
	{
		ft_putstr_fd("Error tokenizing input.\n", 2);
		return (NULL);
	}
	expand(g_data.env_list, tokens);
	ast = NULL;
	curr = create_ast_node();
	ast = curr;
	if (!curr)
		return (ft_putstr_fd("Error creating AST node.\n", 2), NULL);
	while (current)
	{
		if (current->type == TOKEN_EOF)
			break ;
		else if (current->type == TOKEN_WORD)
		{
			if (!handle_word_token(curr, current))
				return (free_tokens(tokens), NULL);
		}
		else if (ft_token_is_redirection(current->type))
		{
			ret = handle_redirection_token(curr, current);
			if (ret == 0)
				return (free_tokens(tokens), NULL);
			if (ret == 2)
				current = current->next;
		}
		else if (current->type == TOKEN_PIPE)
		{
			if (!handle_pipe_token(&ast, &curr, current))
				return (free_tokens(tokens), NULL);
		}
		else
		{
			ft_putstr_fd("Unexpected token type: ", 2);
			ft_putnbr_fd(current->type, 2);
			ft_putstr_fd("\n", 2);
			return (free_tokens(tokens), NULL);
		}
		current = current->next;
	}
	free_tokens(tokens);
	return (ast);
}
