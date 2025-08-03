#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type, int is_quoted)
{
	t_token	*new_token;

	new_token = ft_malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	new_token->is_quoted = is_quoted;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
	}
	else
	{
		current = *head;
		while (current->next)
		{
			current = current->next;
		}
		current->next = new_token;
		new_token->prev = current;
	}
}

void	free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		current = next;
	}
}

int	condition6(t_ddata *ddata, const char *input, int *i, int in_quote)
{
	t_token	*new_token;
	char	*substr;

	if (ddata->len > 0)
	{
		substr = ft_substr(input, *i, ddata->len);
		if (!substr)
			return (0);
		new_token = create_token(substr, TOKEN_WORD, in_quote);
		if (!new_token)
		{
			free(substr);
			return (0);
		}
		add_token(&ddata->head, new_token);
		*i += ddata->len;
		ddata->len = 0;
	}
	if (ddata->ptr[*i + 1] == '>')
	{
		add_token(&ddata->head, create_token(NULL, TOKEN_APPEND, 0));
		*i += 2;
	}
	else
	{
		add_token(&ddata->head, create_token(NULL, TOKEN_REDIRECT_OUT, 0));
		(*i)++;
	}
	return (1);
}

int	condition5(t_ddata *ddata, const char *input, int *i, int in_quote)
{
	t_token	*new_token;
	char	*substr;

	if (ddata->len > 0)
	{
		substr = ft_substr(input, *i, ddata->len);
		if (!substr)
			return (0);
		new_token = create_token(substr, TOKEN_WORD, in_quote);
		if (!new_token)
		{
			free(substr);
			return (0);
		}
		add_token(&ddata->head, new_token);
		*i += ddata->len;
		ddata->len = 0;
	}
	if (ddata->ptr[*i + 1] == '<')
	{
		add_token(&ddata->head, create_token(NULL, TOKEN_HEREDOC, 0));
		*i += 2;
	}
	else
	{
		add_token(&ddata->head, create_token(NULL, TOKEN_REDIRECT_IN, 0));
		(*i)++;
	}
	return (1);
}

int	condition4(t_ddata *ddata, const char *input, int *i, int in_quote)
{
	t_token	*new_token;
	char	*substr;

	if (ddata->len > 0)
	{
		substr = ft_substr(input, *i, ddata->len);
		if (!substr)
			return (0);
		new_token = create_token(substr, TOKEN_WORD, in_quote);
		if (!new_token)
		{
			free(substr);
			return (0);
		}
		add_token(&ddata->head, new_token);
		*i += ddata->len;
		ddata->len = 0;
	}
	add_token(&ddata->head, create_token(NULL, TOKEN_PIPE, 0));
	(*i)++;
	return (1);
}

int	condition3(t_ddata *ddata, const char *input, int *i, int in_quote)
{
	t_token	*new_token;
	char	*substr;

	if (ddata->len > 0)
	{
		substr = ft_substr(input, *i, ddata->len);
		if (!substr)
			return (0);
		new_token = create_token(substr, TOKEN_WORD, in_quote);
		if (!new_token)
		{
			free(substr);
			return (0);
		}
		add_token(&ddata->head, new_token);
		*i += ddata->len;
		ddata->len = 0;
	}
	while (ddata->ptr[*i] == ' ' || ddata->ptr[*i] == '\t')
		(*i)++;
	return (1);
}

int	condition2(t_ddata *ddata, int *i, int *in_quote)
{
	char	quote_char;

	if (ddata->ptr[*i + ddata->len] == '\'')
		*in_quote = 1;
	else
		*in_quote = 2;
	quote_char = ddata->ptr[*i + ddata->len];
	ddata->len++;
	while (ddata->ptr[*i + ddata->len] && ddata->ptr[*i
		+ ddata->len] != quote_char)
		ddata->len++;
	if (ddata->ptr[*i + ddata->len] == quote_char)
		ddata->len++;
	return (1);
}

int	condition1(t_ddata *ddata, const char *input, int *i, int in_quote)
{
	t_token	*new_token;
	t_token	*eof_token;
	char	*substr;

	if (ddata->len > 0)
	{
		substr = ft_substr(input, *i, ddata->len);
		if (!substr)
			return (0);
		new_token = create_token(substr, TOKEN_WORD, in_quote);
		if (!new_token)
		{
			free(substr);
			return (0);
		}
		add_token(&ddata->head, new_token);
	}
	eof_token = create_token(NULL, TOKEN_EOF, 0);
	if (!eof_token)
		return (0);
	add_token(&ddata->head, eof_token);
	return (1);
}

t_token	*tokenize_loop(t_ddata *ddata, const char *input)
{
	int	i;
	int	in_quote;

	i = 0;
	in_quote = 0;
	while (1)
	{
		if (ddata->ptr[i + ddata->len] == '\0')
		{
			if (!condition1(ddata, input, &i, in_quote))
				return (NULL);
			break ;
		}
		if ((ddata->ptr[i + ddata->len] == '\'' || ddata->ptr[i
				+ ddata->len] == '\"') && (condition2(ddata, &i, &in_quote)))
			continue ;
		if ((ddata->ptr[i + ddata->len] == ' ' || ddata->ptr[i
				+ ddata->len] == '\t'))
		{
			if (!condition3(ddata, input, &i, in_quote))
				return (NULL);
			continue ;
		}
		if (ddata->ptr[i + ddata->len] == '|')
		{
			if (!condition4(ddata, input, &i, in_quote))
				return (NULL);
			continue ;
		}
		if (ddata->ptr[i + ddata->len] == '<')
		{
			if (!condition5(ddata, input, &i, in_quote))
				return (NULL);
			continue ;
		}
		if (ddata->ptr[i + ddata->len] == '>')
		{
			if (!condition6(ddata, input, &i, in_quote))
				return (NULL);
			continue ;
		}
		ddata->len++;
	}
	return (ddata->head);
}

t_token	*tokenize_input(const char *input)
{
	t_ddata	*ddata;
	t_token	*head;

	ddata = ft_malloc(sizeof(t_ddata));
	if (!ddata)
		return (NULL);
	ddata->head = NULL;
	ddata->len = 0;
	ddata->ptr = input;
	head = tokenize_loop(ddata, input);
	return (head);
}

t_token	*tokenize(const char *input)
{
	t_token	*tokens;

	if (!input || !*input)
		return (NULL);
	if (unclosed_quotes(input))
	{
		ft_putstr_fd("minishell: unclosed quotes\n", 2);
		return (NULL);
	}
	tokens = tokenize_input(input);
	if (!tokens)
	{
		ft_putstr_fd("minishell: error tokenizing input\n", 2);
		return (NULL);
	}
	return (tokens);
}

char	*ft_token_gettype(t_token_type type)
{
	switch (type)
	{
	case TOKEN_WORD:
		return ("WORD");
	case TOKEN_PIPE:
		return ("PIPE");
	case TOKEN_REDIRECT_IN:
		return ("REDIRECT_IN");
	case TOKEN_REDIRECT_OUT:
		return ("REDIRECT_OUT");
	case TOKEN_APPEND:
		return ("APPEND");
	case TOKEN_HEREDOC:
		return ("HEREDOC");
	case TOKEN_EOF:
		return ("EOF");
	default:
		return ("UNKNOWN");
	}
}

void	print_tokens(t_token *tokens)
{
	(void)tokens;
}