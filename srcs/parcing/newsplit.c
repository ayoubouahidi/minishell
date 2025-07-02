/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newsplit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:56:43 by ayouahid          #+#    #+#             */
/*   Updated: 2025/06/19 13:56:45 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"
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