#include "../includes/minishell.h"

t_data g_data;

void multi_to_single_space(char **av, char *res, int ac)
{
  int(i), (j), (k);

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

void handle_sigint(int sig)
{
  (void)sig;
  printf("\n");
  rl_replace_line("", 0);
  rl_on_new_line();
  rl_redisplay();
}

void handle_sigquit(int sig)
{
  (void)sig;
  // Ignore SIGQUIT (Ctrl-\)
  write(1, "\b\b  \b\b", 6);
}

int main(int ac, char **av, char **env)
{
  char *input;
  t_command *cmd;

  signal(SIGINT, handle_sigint);
  signal(SIGQUIT, handle_sigquit);
  (void)av;
  if (ac > 1)
    return (0);

  g_data.env_list = env_from_array(env);
  while (1)
  {
    input = readline("minishell> ");
    if (input == NULL) // Handle EOF (Ctrl+D)
      break;
    if (*input == '\0') // Handle empty input
    {
      free(input);
      continue;
    }
    add_history(input);
    if (is_blank_line(input) || !check_syntax(input))
    {
      free(input);
      continue;
    }
    cmd = parse_input(input); // Parse the input into a command structure
    if (cmd == NULL)          // Handle parsing errors
    {
      free(input);
      continue;
    }
    shell_loop(cmd, env); // Execute the command(s)
    free_commands(cmd);   // Free the command structure
    free(input);
  }
  return 0;
}

// int main(int ac, char **av, char **env)
// {
//   char *input;
//   t_command *cmd;

//   (void)av;
//   if (ac > 1)
//     return (0);
//   while (1)
//   {
//     // printf("minishell> ");
//     input = readline("minishell> "); // Read input from the user
//     if (!input)                      // Handle EOF (Ctrl+D)
//     {
//       printf("\n");
//       break;
//     }

//     if (strlen(input) == 0) // Skip empty input
//     {
//       free(input);
//       continue;
//     }

//     add_history(input);

//     cmd = parse_input(input); // Parse the input into a command structure
//     if (cmd == NULL)          // Handle parsing errors
//     {
//       free(input);
//       continue;
//     }
//     shell_loop(cmd, env); // Execute the command(s)
//     free_commands(cmd);   // Free the command structure
//     free(input);
//   }

//   return 0;
// }
