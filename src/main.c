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
  char *res;
  int i;

  if (ac < 2)
    return (1);
  res = malloc(sizeof(char) * (strlen(av[1]) + 1));
  if (!res)
    return (1);
  multi_to_single_space(av, res, ac);
  i = 0;
  while (res[i])
  {
    if (res[i] == ' ')
      res[i] = '\0';
    i++;
  }
  free(res);

  // Initialize the shell
  signal(SIGINT, SIG_IGN); // Ignore Ctrl+C
  signal(SIGQUIT, SIG_IGN); // Ignore Ctrl+\
{
  char *input;
  t_command *cmd;

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