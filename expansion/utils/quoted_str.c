#include "../expansion.h"

int	process_quoted_string(char **value, int *len)
{
	char	c;

	c = **value;
	(*value)++;
	while (**value && **value != c)
	{
		(*len)++;
		(*value)++;
	}
	if (**value == c)
		(*value)++;
	return (0);
}
