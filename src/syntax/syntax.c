#include "../../includes/minishell.h"

bool    check_syntax(const char *input)
{
    int i = 0;
    while (input[i])
    {
        if (input[i] == '|' && unclosed_quotes(input) == 0 && pipe_syntax(input))
            return 2;
        if ((input[i] == '<' || input[i] == '>') && unclosed_quotes(input) == 0
            && file_syntax(input))
            return 2;
        i++;
    }
    return 0;
}

// int check_syntax(t_lexer *lex)
// {
//     while (lex)
//     {
//         if (lex->type == '|' && lex->in_quotes == 0 && pipe_syntax(lex))
//             return 2;
//         if ((/*lex->type == '<<' || lex->type == '>>' ||*/
//             lex->type == '<' || lex->type == '>') &&
//             lex->in_quotes == 0 && files_syntax(lex))
//             return 2;
//         lex = lex->next;
//     }
//     return 0;
// }