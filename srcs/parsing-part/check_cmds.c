











#include "../../includes/minishell.h"

int	check_cmds(t_token *tokens)
{
	t_token	*tmp_tokens;

	tmp_tokens = tokens;
	
	
	if (tmp_tokens && tmp_tokens->type == TOKEN_PIPE)
		return (0);
		
	while (tmp_tokens)
	{
		if (tmp_tokens->type == TOKEN_PIPE && tmp_tokens->next == NULL)
			return (0);
		if (tmp_tokens->type == TOKEN_PIPE)
		{
			tmp_tokens = tmp_tokens->next;
			if (tmp_tokens && tmp_tokens->type == TOKEN_PIPE)
				return (0);
		}
		tmp_tokens = tmp_tokens->next;
	}
	return (1);
}
