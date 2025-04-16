#include "../includes/utils.h"
#include "../includes/minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int ft_strlen(const char *str)
{
    int len = 0;
    while (str[len])
        len++;
    return len;
}