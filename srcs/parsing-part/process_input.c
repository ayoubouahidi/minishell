











#include "../../includes/minishell.h"

char	*process_input(char *input, int *i, t_quote_type *quote_type)
{
	char	*value;

	if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<')
		value = get_operator(input, i, quote_type);
	else
		value = get_word(input, i, quote_type);
	if (!value)
		return (NULL);
	return (value);
}
