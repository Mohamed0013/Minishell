#include "../../includes/minishell.h"

bool file_syntax(const char *input) {
    int i = 0;
    while (input[i]) {
        if (input[i] == '<' || input[i] == '>') {
            // Check for invalid combinations (e.g., <<<, <>, >|)
            if ((input[i] == '<' && input[i+1] == '>') || 
                (input[i] == '>' && input[i+1] == '|')) {
                printf("Error: Invalid redirection syntax\n");
                return false;
            }
            // Skip valid << or >>
            if (input[i+1] == input[i]) 
                i++;
            // Check if next char is a valid filename
            int j = i + 1;
            while (input[j] == ' ' || input[j] == '\t') 
                j++;
            if (!input[j] || strchr("<>|&", input[j])) {
                printf("Error: Missing filename after redirection\n");
                return false;
            }
        }
        i++;
    }
    return true;
}

// bool file_syntax(const char *input)
// {
//     int i = 0;

//     while (input[i])
//     {
//         if ((input[i] == '<' || input[i] == '>') && (input[i + 1] == '<' || input[i + 1] == '>'))
//         {
//             printf("Error: Invalid file redirection syntax\n");
//             return (false);
//         }
//         if ((input[i] == '<' || input[i] == '>') && (input[i + 1] == '\0' || input[i + 1] == '|' || input[i + 1] == '&'))
//         {
//             printf("Error: Missing file after redirection\n");
//             return (false);
//         }
//         i++;
//     }
//     return (true);
// }

// int files_syntax(t_lexer *lex)
// {
//     char *msg = "minishell: syntax error near unexpected token `newline'";

//     lex = lex->next;
//     while (lex && lex->type == ' ')
//         lex = lex->next;

//     if (!lex || lex->type == '<' || lex->type == '>'
//         /*|| lex->type == '<<' || lex->type == '>>'*/)
//     {
//         // ft_putendl_fd(msg, 2);
//         printf("%s\n" ,msg);
//         // g_env->exit_status = 2;
//         return 2;
//     }
//     return 0;
// }
