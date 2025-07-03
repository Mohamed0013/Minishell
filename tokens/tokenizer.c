#include "../includes/minishell2.h"
#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type, int is_quoted)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	new_token->is_quoted = is_quoted;
	new_token->next = NULL;
	return (new_token);
}
void add_token(t_token **head, t_token *new_token)
{
	if (!head || !new_token)
		return ;
	if (!*head) {
		*head = new_token;
	} else {
		t_token *current;

		current = *head;
		while (current->next) {
			current = current->next;
		}
		current->next = new_token;
		new_token->prev = current; // Set the previous pointer
		// new_token->next = NULL; // Ensure the new token's next pointer is NULL
	}
}
void free_tokens(t_token *head)
{
	t_token *current;

	current = head;
	while (current) {
		t_token *next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
t_token *tokenize_input(const char *input)
{
	t_token *head = NULL;
	const char *ptr = input;
	size_t i = 0;
	size_t len = 0;
	int in_quote = 0; // Track if we are inside quotes
	while (1) {
		if (ptr[i + len] == '\0') {
			if (len > 0) {
				add_token(&head, create_token(ft_substr(input, i, len), TOKEN_WORD, in_quote));
			}
			add_token(&head, create_token(NULL, TOKEN_EOF, in_quote)); // Add EOF token
			break; // End of input
		}
		if (ptr[i + len] == '\'' || ptr[i + len] == '\"')
		{
			if (ptr[i + len] == '\'')
				in_quote = 1;
			else
				in_quote = 2;
			char quote_char = ptr[i + len];
			len++;
			while (ptr[len + i] && ptr[i + len] != quote_char) {
				len++;
			}
			if (ptr[i + len] == quote_char) {
				len++; // Move past the closing quote
			}
			continue; // Skip the rest of the loop
		}
		if (ptr[i + len] == ' ' || ptr[i + len] == '\t')
		{
			if (len > 0)
			{
				add_token(&head, create_token(ft_substr(input, i, len), TOKEN_WORD, in_quote));
				i += len; // Move past the word
				len = 0; // Reset length for the next word
			}
			while (ptr[i + len] == ' ' || ptr[i + len] == '\t') {
				i++; // Skip spaces and tabs
			}
			continue; // Skip to the next iteration
		}
		if(ptr[i + len] == '|') {
			if (len > 0) {
				add_token(&head, create_token(ft_substr(input, i, len), TOKEN_WORD, in_quote));
				i += len; // Move past the word
				len = 0; // Reset length for the next word
			}
			add_token(&head, create_token(NULL, TOKEN_PIPE, in_quote));
			i++;
			continue;
		}
		if(ptr[i + len] == '<') {
			if (len > 0) {
				add_token(&head, create_token(ft_substr(input, i, len), TOKEN_WORD, in_quote));
				i += len; // Move past the word
				len = 0; // Reset length for the next word
			}
			if (ptr[i + 1] == '<') {
				add_token(&head, create_token(NULL, TOKEN_HEREDOC, in_quote));
				i += 2;
			} else {
				add_token(&head, create_token(NULL, TOKEN_REDIRECT_IN, in_quote));
				i++;
			}
			continue;
		}
		if(ptr[i + len] == '>') {
			if (len > 0) {
				add_token(&head, create_token(ft_substr(input, i, len), TOKEN_WORD, in_quote));
				i += len; // Move past the word
				len = 0; // Reset length for the next word
			}
			if (ptr[i + 1] == '>') {
				add_token(&head, create_token(NULL, TOKEN_APPEND, in_quote));
				i += 2;
			} else {
				add_token(&head, create_token(NULL, TOKEN_REDIRECT_OUT, in_quote));
				i++;
			}
			continue;
		}
		len++;
		// ptr++;
	}
	return head;
}

t_token *tokenize(const char *input)
{
	if (!input || !*input) return NULL;

	// Check for unclosed quotes
	if (unclosed_quotes(input)) {
		ft_putstr_fd("minishell: unclosed quotes\n", 2);
		return NULL;
	}

	// Tokenize the input
	t_token *tokens = tokenize_input(input);
	if (!tokens) {
		ft_putstr_fd("minishell: error tokenizing input\n", 2);
		return NULL;
	}

	return tokens;
}

char *ft_token_gettype(t_token_type type)
{
	switch (type)
	{
		case TOKEN_WORD: return "WORD";
		case TOKEN_PIPE: return "PIPE";
		case TOKEN_REDIRECT_IN: return "REDIRECT_IN";
		case TOKEN_REDIRECT_OUT: return "REDIRECT_OUT";
		case TOKEN_APPEND: return "APPEND";
		case TOKEN_HEREDOC: return "HEREDOC";
		case TOKEN_EOF: return "EOF";
		default: return "UNKNOWN";
	}
}

void print_tokens(t_token *tokens)
{
	t_token *current = tokens;
	int i = 0;
	if (!current)
	{
		printf("No tokens to display.\n");
		return;
	}
	while (current)
	{
		printf("Token[%d]: Type: %20s : value : %s, in_quote: %d\n", i++,
			ft_token_gettype(current->type), current->value, current->is_quoted);
		current = current->next;
	}
}
