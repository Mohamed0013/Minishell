#ifndef EXPANSION_H
#define EXPANSION_H

#include "minishell.h"
#include "parsing.h"
#include "minishell2.h"

int ft_keycmp(char * keyvalue,char* key);

void expand(t_env *env, t_token *token);
// char *expand_arg(const char *arg, t_env *env);
char *get_env_value(t_env *env,char *var_name);
int mask_len(t_token *token);
t_env *make_env(const char *name, const char *value);
void free_env(t_env *env);
void print_args(t_token *args);
void test_expansion(const char *input, t_list *env);

#endif // EXPANSION_H