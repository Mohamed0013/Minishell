#include "../../includes/minishell.h"

bool file_syntax(const char *input)
{
    int i;

    i = 0;
    bool in_single = false, in_double = false;

    while (input[i])
    {
        // Update quote states
        if (input[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (input[i] == '"' && !in_single)
            in_double = !in_double;
        if (!in_single && !in_double)
        {
            if (input[i] == '<' || input[i] == '>')
            {
                // Check for invalid combinations (e.g., <>, >|)
                if ((input[i] == '<' && input[i+1] == '>') || 
                    (input[i] == '>' && input[i+1] == '|'))
                    {
                        printf("Syntax error: invalid operator\n");
                        return false;
                    }
                // Handle heredoc (<<)
                if (input[i] == '<' && input[i+1] == '<')
                {
                    i += 2;
                    while (input[i] == ' ' || input[i] == '\t')
                        i++;
                    if (!input[i] || ft_strchr("<>|", input[i]))
                    {
                        printf("Syntax error: missing heredoc delimiter\n");
                        return false;
                    }
                    continue;
                }
                // Skip valid >> or <<
                if (input[i+1] == input[i])
                    i++;
                // Check filename exists
                int j = i + 1;
                while (input[j] == ' ' || input[j] == '\t')
                    j++;
                if (!input[j] || ft_strchr("<>|", input[j]))
                {
                    printf("Syntax error: missing filename\n");
                    return false;
                }
            }
        }
        i++;
    }
    return true;
}

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
