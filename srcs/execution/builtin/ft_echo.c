#include "minishell.h"

static int	has_only_n(char *arg)
{
	int	i;

	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **args)
{
	int (n_flag), (i), (j);

	n_flag = 0;
	i = 1;
	j = 0;
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		if (has_only_n(args[i]))
			n_flag = 1;
		else
			break ;
		i++;
	}
	while (args[i])
	{
		if (j != 0)
			ft_putchar_fd(' ', 1);
		ft_putstr_fd(args[i], 1);
		j++;
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
