#include "../includes/minishell.h"

t_data g_data;

/*
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
*/
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

void print_argv(t_command *cmd)
{
  t_command *current = cmd;

  while (current)
  {
    // printf("Command: %s\n", current->command);
    if (current->arguments)
    {
      printf("Arguments: \n");
      for (int i = 0; current->arguments[i]; i++)
      {
        printf("argv[%d]: %s\n", i, current->arguments[i]);
      }
      printf("\n");
    }
    current = current->next;
  }
  printf("=================================\n");
}

void free_env_list(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = tmp;
    }
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
    cmd = parse_input(input, env); // Parse the input into a command structure
    if (cmd == NULL)          // Handle parsing errors
    {
      free(input);
      continue;
    }
    // print_argv(cmd);
    shell_loop(cmd, env); // Execute the command(s)
    free_commands(cmd);   // Free the command structure
    free(input);
  }
  free_env_list(g_data.env_list); // Free the environment list
  // printf("exit\n");
  // Free any remaining resources
  // Note: You may want to implement a proper cleanup function
  // to free all allocated memory in g_data if necessary.
  // for (int i = 0; i < g_data.env_count; i++)
  // {
  //   free(g_data.env[i]);
  // }
  // free(g_data.env);
  // free(g_data.env_list);
  return 0;
}
