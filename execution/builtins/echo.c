#include "../../includes/minishell.h"

static int	empty_arg(char **arguments)
{
	if (!arguments || !arguments[0])
	{
		printf("\n");
		return (1);
	}
	return (0);
}

void	execute_echo(char **arguments, int status)
{
	int (i) = 1;
	int (newline) = 1;
	if (empty_arg(arguments) == 1)
		return ;
	if (arguments[i] && strcmp(arguments[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	else if (arguments[i] && strcmp(arguments[i], "$?") == 0)
	{
		printf("%d", status);
		if (newline)
			printf("\n");
		return ;
	}
	while (arguments[i])
	{
		printf("%s", arguments[i]);
		if (arguments[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
