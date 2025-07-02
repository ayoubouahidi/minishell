#include "../../includes/minishell.h"
#include <signal.h>

static void	write_in_here_doc_file(t_redirections *redir, t_env *env, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			if (!line)
				printf("minishell: warning: here-document "
					"delimited by end-of-file (wanted `%s')\n",
					redir->file);
			break ;
		}
		if (redir->quote_type == NO_QUOTE)
			line = expand_the_heredoc(line, env);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

static void	handle_child_heredoc(t_command *cmd, t_env *env, char **files,
		int start_idx)
{
	t_redirections	*redir;
	int				fd;
	int				local_idx;

	redir = cmd->redirections;
	local_idx = start_idx;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC && redir->file)
		{
			fd = open(files[local_idx], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("heredoc open");
				exit(1);
			}
			write_in_here_doc_file(redir, env, fd);
			close(fd);
			local_idx++;
		}
		redir = redir->next;
	}
	exit(0);
}

static void	here_doc_process(t_command *cmds, t_env *env, char **files)
{
	int	pid;
	int	nredir;
	int	idx;
	int	start_idx;
	int	status;

	idx = 0;
	while (cmds)
	{
		nredir = count_redirections(cmds);
		if (nredir > 0)
		{
			start_idx = idx;
			pid = fork();
			if (pid < 0)
				return ;
			if (pid == 0)
			{
				handle_child_heredoc(cmds, env, files, start_idx);
			}
			else
			{
				ign_ctrl_c_with_exit_status(pid, &status, &cmds->signal_detected);
				signal(SIGINT, SIG_DFL);
			}
			cmds->here_doc_file = ft_strdup(files[start_idx + nredir - 1]);
			idx = start_idx + nredir;
		}
		if (cmds->signal_detected)
			break ;
		cmds = cmds->next;
	}
}

int	heredoc(t_command *cmds, t_env *env)
{
	t_command	*tmp_cmds;
	char		**files;
	int			total_here_doc;

	files = NULL;
	total_here_doc = 0;
	tmp_cmds = cmds;
	total_here_doc = count_here_doc(tmp_cmds, &files);
	if (total_here_doc > 16)
		return (-1);
	here_doc_process(tmp_cmds, env, files);
	unlink_files(total_here_doc, files);
	return (0);
}
char *get_tmp_file(void)
{
    static int counter = 0;
    char *num_str;
    char *filename;
    
    num_str = ft_itoa(counter++);
    filename = ft_strjoin("/tmp/heredoc_", num_str);
    free(num_str);
    return filename;
}

