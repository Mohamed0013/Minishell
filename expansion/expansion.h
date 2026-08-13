/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:37 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:37 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

typedef struct s_expand_ctx
{
	const char	*arg;
	t_env		*env;
	char		*result;
	size_t		i;
	size_t		j;
	int			in_squote;
	int			in_dquote;
}				t_expand_ctx;

int				process_quoted_string(char **value, int *len);
bool			prev_not_heredoc(t_token *token);
bool			prev_not_redirect(t_token *token);
char			*expand_arg(const char *arg, t_env *env);
char			*get_env_value(t_env *env, char *var_name);
int				mask_len(t_token *token);
int				handle_single_quote(t_expand_ctx *ctx);
int				handle_double_quote(t_expand_ctx *ctx);
int				handle_variable_expansion(t_expand_ctx *ctx);
void			init_expand_ctx(t_expand_ctx *ctx, const char *arg, t_env *env);
int				process_env_variable(char **value, int *len);

#endif
