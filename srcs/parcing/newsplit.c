/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newsplit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:56:43 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/05 20:24:00 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"
#include <stdbool.h>
#include "../../includes/parser.h"

char    **extracting_cmd(t_token *token, int i)
{
    int        start;
    int        count;
    char    **args;
    int        j;

    start = i;
    count = 0;
    while (token[i].value && token[i].type != PIPE)
    {
        if (token[i].type == WORD)
            count++;
        i++;
    }
    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return (NULL);
    j = 0;
    while (start < i)
    {
        if (token[start].type == WORD)
            args[j++] = ft_strdup(token[start].value);
        start++;
    }
    args[j] = NULL;
    return (args);
}


// char *is_word(t_lexer *lexer)
// {
//     t_lexer tmp;
//     int count;
//     char *value;

//     tmp = *lexer;
//     count = 0;
    
//     // First pass: count all characters in the word
//     while (ft_isalnum(tmp.c) || tmp.c == '=' || tmp.c == '"')
//     {
//         if (tmp.c == '"')
//         {
//             // Count the opening quote
//             count++;
//             increment_using_index(&tmp);
            
//             // Count everything until closing quote
//             while (tmp.c != '"' && tmp.c != '\0')
//             {
//                 count++;
//                 increment_using_index(&tmp);
//             }
            
//             // Count the closing quote if present
//             if (tmp.c == '"')
//             {
//                 count++;
//                 increment_using_index(&tmp);
//             }
//         }
//         else
//         {
//             count++;
//             increment_using_index(&tmp);
//         }
//     }
    
//     // Allocate memory
//     value = (char *)malloc(count + 1);
//     if (!value)
//         return NULL;
    
//     // Second pass: copy characters
//     count = 0;
    
//     while (ft_isalnum(lexer->c) || lexer->c == '=' || lexer->c == '"' || lexer->c == '\'')
//     {
//         if (lexer->c == '"' || lexer->c == '\'')
//         {
//             // Copy the opening quote
//             value[count++] = lexer->c;
//             increment_using_index(lexer);
            
//             // Copy everything until closing quote
//             while ((lexer->c != '"' || lexer->c == '\'') && lexer->c != '\0')
//             {
//                 value[count++] = lexer->c;
//                 increment_using_index(lexer);
//             }
            
//             // Copy the closing quote if present
//             if (lexer->c == '"' || lexer->c == '\'')
//             {
//                 value[count++] = lexer->c;
//                 increment_using_index(lexer);
//             }
//         }
//         else
//         {
//             value[count++] = lexer->c;
//             increment_using_index(lexer);
//         }
//     }
    
//     value[count] = '\0';
//     return (value);
// }