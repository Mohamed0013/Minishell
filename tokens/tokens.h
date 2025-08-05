#ifndef TOKENS_H
# define TOKENS_H

# include "minishell.h"

int		condition1(t_ddata *ddata, const char *input, int *i, int in_quote);
int		condition2(t_ddata *ddata, int *i, int *in_quote);
int		condition3(t_ddata *ddata, const char *input, int *i, int in_quote);
int		condition4(t_ddata *ddata, const char *input, int *i, int in_quote);
int		condition5(t_ddata *ddata, const char *input, int *i, int in_quote);
int		condition6(t_ddata *ddata, const char *input, int *i, int in_quote);
void	free_tokens(t_token *head);
void	add_token(t_token **head, t_token *new_token);
t_token	*create_token(char *value, t_token_type type, int is_quoted);
bool	unclosed_quotes(const char *input);

#endif