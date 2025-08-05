#include "../expansion.h"

int	process_env_variable(char **value, int *len)
{
	int		i;
	char	c;
	char	*env_value;

	(*value)++;
	i = 0;
	while (isalnum((*value)[i]) || (*value)[i] == '_')
		i++;
	c = (*value)[i];
	(*value)[i] = '\0';
	env_value = getenv(*value);
	*len += strlen(env_value);
	(*value)[i] = c;
	*value += i;
	return (0);
}
