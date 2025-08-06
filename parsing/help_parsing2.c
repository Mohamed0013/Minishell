#include "minishell.h"

// int	handle_heredoc(char *delimiter)
// {
//     int		fd;
//     char	*line;

//     fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if (fd < 0)
//     {
//         perror("Error opening heredoc file");
//         return (-1);
//     }

//     while (1)
//     {
//         line = readline("> ");
//         if (!line)
//             break ; // EOF or error

//         if (ft_strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break ; // End of heredoc
//         }

//         write(fd, line, ft_strlen(line));
//         write(fd, "\n", 1);
//         free(line);
//     }

//     close(fd);
//     return (0);
// }
