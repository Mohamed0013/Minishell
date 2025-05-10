#include "../includes/minishell.h"

// To detect Emty input
bool    is_blank_line(const char *s)
{
    size_t  i = 0;
    if (!s)
        return (true);
    while (s[i])
    {
        if (!isspace((unsigned char)s[i]))
            return (false);
        i++;
    }
    return (true);
}

void    print_syntax_error()
{
    perror("minishell: syntax error!\n");
    // extern int g_exit_status;
    // g_exit_status = 2;
}

void free_lexer(const char **str)
{
    if (!str || !*str)
        return;

    free((void *)*str); // Cast away const to free safely
    *str = NULL;        // Avoid dangling pointer
}


// void free_lexer(t_lexer **lexer)
// {
//     t_lexer *current;
//     t_lexer *next;

//     if (!lexer || !*lexer)
//         return;

//     current = *lexer;
//     while (current)
//     {
//         next = current->next;
//         free(current->value); // Free the token value
//         free(current);        // Free the lexer node
//         current = next;
//     }
//     *lexer = NULL; // Set the pointer to NULL to avoid dangling references
// }

void  multi_to_single_space(char **av, char *res, int ac)
{
	int (i), (j), (k);

	i = 1;
	k = 0;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			while (av[i][j] && av[i][j] == ' ')
				j++;
			while (av[i][j] && av[i][j] != ' ')
				res[k++] = av[i][j++];
			while (av[i][j] && av[i][j] == ' ')
				j++;
			if (av[i][j] != '\0')
				res[k++] = ' ';
		}
		if (i < ac - 1)
			res[k++] = ' ';
		i++;
	}
	res[k] = '\0';
}
