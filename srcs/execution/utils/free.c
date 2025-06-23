#include "../../../includes/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	clean_exit(t_data *data, int exit_code)
{
	if (data->cmd)
		free_cmd(data->cmd);
	if (data->env)
		free_env(data->env);
	exit(exit_code);
}

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_all(t_data *data)
{
	if (data->cmd)
	{
		free_args(data->cmd->args);
		free(data->cmd);
	}
}

void	cleanup_child_resources(char *path, char **envp)
{
	free(path);
	free_array(envp);
}

void	free_cmd(t_command *cmd)
{
	t_command		*tmp;
	t_redirections	*redir;
	t_redirections	*redir_tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_args(cmd->args);
		if (cmd->here_doc_file)
			free(cmd->here_doc_file);
		redir = cmd->redirections;
		while (redir)
		{
			redir_tmp = redir->next;
			if (redir->file)
				free(redir->file);
			free(redir);
			redir = redir_tmp;
		}
		free(cmd);
		cmd = tmp;
	}
}
