#include "../../includes/minishell.h"

bool pipe_syntax(const char *input)
{
    int (i), (j);
    // prev_pipe needs to checked
    // bool prev_pipe = false, in_single = false, in_double = false;
    bool in_single = false, in_double = false;

    i = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (input[i] == '"' && !in_single)
            in_double = !in_double;

        if (!in_single && !in_double)
        {
            if (input[i] == '|')
            {
                // if (prev_pipe || i == 0 || input[i+1] == '\0')
                if (i == 0 || input[i+1] == '\0')
                {
                    printf("Syntax error near '|'\n");
                    return false;
                }
                // Check next non-space token
                j = i + 1;
                while (input[j] == ' ' || input[j] == '\t')
                    j++;
                if (ft_strchr("|><", input[j]))
                {
                    printf("Syntax error after '|'\n");
                    return false;
                }
                // prev_pipe = true;
            }
            // else
                // prev_pipe = false;
        }
        i++;
    }
    return true;
}

// int pipe_syntax(t_lexer *lex)
// {
//     t_lexer *tmp = lex->prev;

//     while (tmp && tmp->type == ' ')
//         tmp = tmp->prev;

//     lex = lex->next;
//     while (lex && lex->type == ' ')
//         lex = lex->next;

//     if (!lex || lex->type == '|' || !tmp || tmp->type == '|' ||
//         tmp->type == '<' || tmp->type == '>' /*|| tmp->type == '<<' || tmp->type == '>>'*/)
//     {
//         // ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
//         printf("minishell: syntax error near unexpected token `|'");

//         // g_env->exit_status = 2;
//         return 2;
//     }
//     return 0;
// }
