#include "../includes/minishell.h"

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

int main(int ac, char **av, char **env)
{
  char *input;
  t_command *cmd;

  (void)av;
  if (ac > 1)
    return (0);
  while (1)
  {
    printf("minishell> ");
    input = readline(NULL); // Read input from the user
    if (!input) // Handle EOF (Ctrl+D)
    {
      printf("\n");
      break;
    }

    if (strlen(input) == 0) // Skip empty input
    {
      free(input);
      continue;
    }
  
    cmd = parse_input(input); // Parse the input into a command structure
    if (cmd == NULL) // Handle parsing errors
    {
      free(input);
      continue;
    }
    shell_loop(cmd, env); // Execute the command(s)
    free_commands(cmd); // Free the command structure
    free(input);
  }

  return 0;
}