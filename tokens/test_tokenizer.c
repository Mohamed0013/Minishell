#include "../includes/minishell2.h"
#include "../includes/minishell.h"

// Forward declarations
// bool unclosed_quotes(const char *input);
// t_token *tokenize(const char *input);

// Implementation of unclosed_quotes function
bool unclosed_quotes(const char *input)
{
    bool in_single_quote = false;
    bool in_double_quote = false;

    while (*input)
    {
        if (*input == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
        }
        else if (*input == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
        }
        input++;
    }

    return in_single_quote || in_double_quote;
}

void test_tokenizer(const char *input, const char *test_name)
{
    printf("\n=== Testing: %s ===\n", test_name);
    printf("Input: \"%s\"\n", input);

    t_token *tokens = tokenize(input);
    if (!tokens)
    {
        printf("Tokenization failed!\n");
        return;
    }

    printf("Tokens:\n");
    print_tokens(tokens);
    free_tokens(tokens);
}

// int main(void)
// {
//     // Test 1: Basic command
//     test_tokenizer("ls'hello'     'world' ||<<<<>>>>", "Basic command");

//     // Test 2: Command with arguments
//     test_tokenizer("ls -la", "Command with arguments");

//     // Test 3: Command with pipe
//     test_tokenizer("ls | grep test", "Command with pipe");

//     // Test 4: Command with redirection
//     test_tokenizer("ls > output.txt", "Command with output redirection");
//     test_tokenizer("cat < input.txt", "Command with input redirection");
//     test_tokenizer("ls >> output.txt", "Command with append redirection");
//     // Test 4.1: Command with redirection and go creazy
//     test_tokenizer("echo $a&'|'\"|<><><<><><><<>><>><\"<>><<<>> ls", "Command with redirection and special characters");

//     // Test 5: Complex command
//     test_tokenizer("ls -la | grep test > output.txt", "Complex command with pipe and redirection");

//     // Test 6: Multiple spaces
//     test_tokenizer("ls    -la    |    grep    test", "Multiple spaces");

//     // Test 7: Empty input
//     test_tokenizer("", "Empty input");

//     // Test 8: Only spaces
//     test_tokenizer("   ", "Only spaces");

//     // Test 9: Quoted strings
//     test_tokenizer("echo \"hello world\"", "Double quoted string");
//     test_tokenizer("echo 'hello world'", "Single quoted string");
//     test_tokenizer("echo \"hello 'world'\"", "Nested quotes");

//     return 0;
// }