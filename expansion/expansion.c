#include "../includes/minishell.h"

char	*get_env_value(t_env *env, char *var_name)
{
	if (!env || !var_name || !*var_name)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->name) == ft_strlen(var_name) && ft_strncmp(env->name,
				var_name, ft_strlen(var_name)) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}

int	mask_len(t_token *token)
{
	int		len;
	int		i;
	char	c;
	char	*value;
	char	*env_value;

	if (!token || !token->value)
		return (0);
	len = 0;
	i = 0;
	value = token->value;
	while (*value)
	{
		if (*value == '\'' || *value == '"')
		{
			c = *value;
			value++;
			while (*value && *value != c)
			{
				len++;
				value++;
			}
			if (*value == c)
				value++;
		}
		else if (*value == '$' && (isalpha(*(value + 1)) || *(value
					+ 1) == '_'))
		{
			value++;
			i = 0;
			while (isalnum(value[i]) || value[i] == '_')
			{
				i++;
			}
			c = value[i];
			value[i] = '\0';
			env_value = getenv(value);
			len += strlen(env_value);
			value[i] = c;
			value += i;
		}
		else
		{
			len++;
			value++;
		}
	}
	return (len);
}

bool	prev_not_heredoc(t_token *token)
{
	if (!token || !token->prev)
		return (true);
	return (token->prev->type != TOKEN_HEREDOC);
}

bool	prev_not_redirect(t_token *token)
{
	if (!token || !token->prev)
		return (true);
	return (!ft_token_is_redirection(token->prev->type));
}

static char	*expand_arg(const char *arg, t_env *env, t_token *token)
{
	size_t	len;
	char	*result;
	size_t	i = 0, j;
	int		in_squote = 0, in_dquote;
	size_t	var_start;
	size_t	var_len;
			char var_name[128];
			const char *val = get_env_value(env, var_name);
	size_t	vlen;

	if (!arg || !*arg || !env)
		return (NULL);
	len = ft_strlen(arg);
	result = malloc(len * 4 + 1);
	if (!result)
		return (NULL);
	i = 0, j = 0;
	in_squote = 0, in_dquote = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' && !in_dquote)
		{
			in_squote = !in_squote;
			i++;
			continue ;
		}
		if (arg[i] == '"' && !in_squote)
		{
			in_dquote = !in_dquote;
			i++;
			continue ;
		}
		if (arg[i] == '$' && !in_squote && prev_not_redirect(token)
			&& (ft_isalpha(arg[i + 1]) || arg[i + 1] == '_'))
		{
			var_start = i + 1;
			var_len = 0;
			while (ft_isalnum(arg[var_start + var_len]) || arg[var_start
				+ var_len] == '_')
			{
				var_len++;
			}
			ft_strlcpy(var_name, arg + var_start, var_len + 1);
			vlen = ft_strlen(val);
			ft_memcpy(result + j, val, vlen);
			j += vlen;
			i = var_start + var_len;
			continue ;
		}
		result[j++] = arg[i++];
	}
	result[j] = '\0';
	return (result);
}

void	expand(t_env *env, t_token *token)
{
	t_token	*current;
	char	*expanded;

	if (!env || !token)
		return ;
	current = token;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_arg(current->value, env, current);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
