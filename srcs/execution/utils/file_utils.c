#include "../../../includes/minishell.h"

/**
 * Safely extracts a filename from a string, handling NULL inputs
 * and trimming whitespace if necessary.
 */
char *get_file_name(char *file_str)
{
    char *trimmed;
    
    if (!file_str)
        return (NULL);
    
    // Trim whitespace and return a new string
    trimmed = ft_strtrim(file_str, " \t\n\r\f\v");
    if (!trimmed)
        return (ft_strdup(file_str)); // Fallback if trim fails
        
    return trimmed;
}
