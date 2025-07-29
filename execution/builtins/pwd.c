#include "../includes/minishell.h"

// Function to implement pwd builtin
int execute_pwd(void)
{
    char *cwd;
    
    // getcwd with NULL allocates memory automatically
    cwd = getcwd(NULL, 0);
    
    if (cwd != NULL)
    {
        // Successfully got current directory
        printf("%s\n", cwd);
        free(cwd);
        return (0);
    }
    else
    {
        // getcwd failed - directory might have been deleted
        if (errno == ENOENT)
        {
            // Directory no longer exists
            fprintf(stderr, "pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
        }
        else if (errno == EACCES)
        {
            // Permission denied
            fprintf(stderr, "pwd: error retrieving current directory: getcwd: Permission denied\n");
        }
        else if (errno == ENAMETOOLONG)
        {
            // Path too long
            fprintf(stderr, "pwd: error retrieving current directory: getcwd: File name too long\n");
        }
        else
        {
            // Other error
            perror("pwd");
        }
        return (1);
    }
}