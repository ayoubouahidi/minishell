#include "../../../includes/minishell.h"

/**
 * Utility function to generate a temporary file name for heredoc
 */
char *get_tmp_file(void)
{
    static int counter = 0;
    char *num_str;
    char *filename;
    
    num_str = ft_itoa(counter++);
    if (!num_str)
        return (NULL);
    filename = ft_strjoin("/tmp/heredoc_", num_str);
    free(num_str);
    return (filename);
}

