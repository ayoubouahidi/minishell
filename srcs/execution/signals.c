/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 20:30:57 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	sig_handler_parent(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

static void	sig_handler_child(int signum)
{
	if (signum == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		g_exit_status = 131;
	}
	if (signum == SIGINT)
	{
		printf("\n");
		g_exit_status = 130;
	}
}

void	signal_parent_handler(void)
{
	signal (SIGQUIT, SIG_IGN);
	signal (SIGINT, sig_handler_parent);
}

void	signal_child_handler(void)
{
	signal(SIGQUIT, sig_handler_child);
	signal(SIGINT, sig_handler_child);
}
