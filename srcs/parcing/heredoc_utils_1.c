/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 16:02:49 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/12 16:02:51 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void	unlink_heredoc_files(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (current->is_heredoc && current->here_doc_file)
			unlink(current->here_doc_file);
		current = current->next;
	}
}
