#include "minishell.h"

volatile sig_atomic_t	*ft_sigint_track(void);

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
			return (2);
		redir->type = current->type;
		redir->filename = ft_strdup(current->next->value);
		ft_gc_add(redir->filename);
		if (!redir->filename)
			return (2);
		else if (redir->type == TOKEN_HEREDOC)
		{
			redir->value = handle_heredoc(redir->filename);
			ft_gc_add(redir->value);
			if (*ft_sigint_track() == 1)
				return (130);
		}
		if (!ft_lst_push(&curr->redirections, redir))
			return (2);
		return (0);
	}
	return (2);
}

static int	handle_pipe_token(t_ast **ast, t_ast **curr, t_token *current)
{
	t_ast	*new_node;

	if (current->next->type != TOKEN_WORD
		&& !ft_token_is_redirection(current->next->type))
	{
		ft_putstr_fd("Syntax error\n", 2);
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

static int	parse_loop(t_token *current, t_ast *curr, t_token *tokens,
		t_ast *ast)
{
	int (ret) = 0;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
		{
			if (!handle_word_token(curr, current))
				return (free_tokens(tokens), 2);
		}
		else if (ft_token_is_redirection(current->type))
		{
			ret = handle_redirection_token(curr, current);
			if (ret != 0)
				return (free_tokens(tokens), ret);
			current = current->next;
		}
		else if (current->type == TOKEN_PIPE)
		{
			if (!handle_pipe_token(&ast, &curr, current))
				return (free_tokens(tokens), 2);
		}
		else
			return (free_tokens(tokens), 2);
		current = current->next;
	}
	return (0);
}

t_ast	*parser(const char *input)
{
	int	ret;

	t_ast(*ast);
	t_ast(*curr);
	t_token(*tokens) = tokenize(input);
	t_token(*current) = tokens;
	if (!tokens)
	{
		g_data.exit_status = 2;
		ft_putstr_fd("Error tokenizing input.\n", 2);
		return (NULL);
	}
	expand(g_data.env_list, tokens);
	curr = create_ast_node();
	ast = curr;
	ret = parse_loop(current, curr, tokens, ast);
	if (ret != 0)
	{
		if (ret == 2)
			ft_putstr_fd("Syntax error: Unexpected token.\n", 2);
		g_data.exit_status = ret;
		return (NULL);
	}
	free_tokens(tokens);
	return (ast);
}
