#include "../includes/minishell.h"

bool	unclosed_quotes(const char *input)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	return (in_single_quote || in_double_quote);
}

void	test_tokenizer(const char *input, const char *test_name)
{
	t_token	*tokens;

	printf("\n=== Testing: %s ===\n", test_name);
	printf("Input: \"%s\"\n", input);
	tokens = tokenize(input);
	if (!tokens)
	{
		printf("Tokenization failed!\n");
		return ;
	}
	printf("Tokens:\n");
	print_tokens(tokens);
	free_tokens(tokens);
}
