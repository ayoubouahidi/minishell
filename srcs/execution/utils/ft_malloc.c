/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:54:34 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 17:55:43 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_malloc(size_t size, int mode)
{
	static t_list	*head_list;
	t_list			*new_node;
	void			*content;

	new_node = NULL;
	content = NULL;
	if (mode == 1)
	{
		content = malloc(size);
		if (!content)
		{
			ft_putstr_fd("malloc Failed: ", STDERR_FILENO);
			ft_malloc(0, 0);
			exit (EXIT_FAILURE);
		}
		new_node = ft_lstnew(content);
		ft_lstadd_back(&head_list, new_node);
	}
	else if (mode == 0)
	{
		ft_lstclear(&head_list, free);
		head_list = NULL;
	}
	return (content);
}
