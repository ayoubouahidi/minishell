#include "../../includes/minishell.h"

// Forward declaration bash nتجنبو l implicit declaration error
static int	external_command(t_data *data);

/*
 * HNA KANDIRO L BUILTINS F CHILD PROCESS O KANRJ3O L STATUS
 * 
 * Ashno katdir:
 * 1. Kan7fdo l stdin o stdout bash nrj3ouhom b3d ma nkmlo
 * 2. Kandiro l redirections (>, <, >>) 9bal ma nshghlo l builtin
 * 3. Kanshghlo l builtin command
 * 4. Kanrj3o l stdin o stdout kima kano 9bal
 * 
 * Limada n7tajou nhfdo stdin/stdout:
 * - 7it l redirections katbdl fihoum
 * - Bash l shell yb9a khdam normal b3d ma l command tkml
 * 
 * Mathalan: echo hello > file
 * - Qbl: stdout kaydkhol terminal
 * - B3d redirection: stdout kaydkhol file
 * - B3d restore: stdout ydrj3 l terminal
 */
static int	handle_builtin(t_data *data)
{
	// kan7fdo l stdin o stdout bash nrj3ouhom m3a b3d
	int	stdin_copy;
	int	stdout_copy;
	int	status;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	
	// kandiro redirections
	if (setup_redirections(data->cmd) < 0)
	{
		close(stdin_copy);
		close(stdout_copy);
		return (1);
	}
	
	// kanshghlo l builtin
	status = execute_builtin(data);
	
	// kanrj3o stdin o stdout kima kano
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	return (status);
}

/*
 * HNA KANSHGHLO COMMAND WA7DA MN GHIR PIPES
 * 
 * Ashno katdir:
 * - Kanshofou ila l command builtin wla external
 * - Ila builtin: kanshghlouha b handle_builtin()
 * - Ila external: kanshghlouha b external_command()
 * 
 * Mathal:
 * - "echo hello" → builtin
 * - "ls -l" → external command
 * - "cd /home" → builtin
 */
int	execute_single_command(t_data *data)
{
	// ila kan builtin kanshghlouh hna
	if (is_builtin(data->cmd->args[0]))
		return (handle_builtin(data));
	// ila makanchx builtin kandozo l external command
	return (external_command(data));
}

/*
 * HNA KANQALBO 3LA L PATH DYAL L COMMAND
 * 
 * Ashno katdir:
 * 1. Ila l command fih slash (/ wla ./) → absolute/relative path:
 *    - Kanshofou ila l file kayn (F_OK)
 *    - Kanshofou ila executable (X_OK)
 *    - Kanrj3o copy dyal l path
 * 
 * 2. Ila makanx fih slash → kanqalbo f PATH environment:
 *    - Kan7awlo nshofou ila kayn f current directory
 *    - Ila makanx, kanqalbo f koll directories f PATH
 * 
 * Mathal:
 * - "./minishell" → relative path
 * - "/bin/ls" → absolute path  
 * - "ls" → kanqalbo f PATH
 */
static char	*get_command_path(t_data *data)
{
	char	*path;

	// ila kan fih slash, ya3ni absolute aw relative path
	if (ft_strchr(data->cmd->args[0], '/'))
	{
		// kanshofou ila l file kayn
		if (access(data->cmd->args[0], F_OK) != 0)
			return (NULL);
		// kanshofou ila executable
		if (access(data->cmd->args[0], X_OK) != 0)
			return (NULL);
		return (ft_strdup(data->cmd->args[0]));
	}
	
	// ila makanchx fih slash, kanqalbo f PATH
	if (access(data->cmd->args[0], X_OK) == 0)
		path = ft_strdup(data->cmd->args[0]);
	else
		path = get_path(data, data->cmd->args[0]);
	
	return (path);
}

/*
 * HNA KANSHGHLO EXTERNAL COMMAND
 * 
 * L steps:
 * 1. Kanqalbo 3la l path dyal l command
 * 2. Ila mal9inahch → error "command not found"
 * 3. Kandir fork() bash nkhlq9o child process
 * 4. F child process:
 *    - Kandiro redirections
 *    - Kan7awlo l environment l array
 *    - Kanshghlo l command b execve()
 * 5. F parent: kantsenna l child process ykml
 * 
 * Exit codes:
 * - 1: fork fshel
 * - 126: execve fshel (permission denied)
 * - 127: command not found
 */
static int	launch_external_command(t_data *data)
{
	char	*path;
	char	**envp;
	pid_t	pid_ch;

	path = get_command_path(data);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(data->cmd->args[0], STDERR_FILENO);
		if (ft_strchr(data->cmd->args[0], '/'))
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		else
			ft_putendl_fd(": command not found", STDERR_FILENO);
		return (127);
	}
	pid_ch = fork();
	if (pid_ch == -1)
	{
		perror("minishell: fork");
		free(path);
		return (1);
	}
	if (pid_ch == 0)
	{
		if (setup_redirections(data->cmd) < 0)
			exit(1);
		envp = env_to_array(data->env);
		execve(path, data->cmd->args, envp);
		perror("minishell: execve");
		cleanup_child_resources(path, envp);
		exit(126);
	}
	waitpid(pid_ch, &data->exit_status, 0);
	free(path);
	return (WEXITSTATUS(data->exit_status));
}

/*
 * HNA KANDIRO EXTERNAL COMMAND
 * 
 * Ashno katdir:
 * - Ila makanx 3ndna args → ghi kandiro redirections
 * - Ila kan 3ndna args → kanshghlo l command
 * 
 * Mathal:
 * - "> file" → ghi redirection mn ghir command
 * - "ls > file" → command m3a redirection
 */
static int	external_command(t_data *data)
{
	// ila makanch 3ndna args, ghi kandiro redirections
	if (!data->cmd->args[0])
	{
		if (setup_redirections(data->cmd) < 0)
			return (1);
		return (0);
	}
	return (launch_external_command(data));
}

/*
 * HADI L FUNCTION RAYSIYA LI KATSHGHEL KOLL SHI
 * 
 * Kifash katkhdem:
 * 1. Kanshofou ila 3ndna command wla la
 * 2. Ila makanx args → ghi redirections
 * 3. Ila kan builtin o makanx pipes → kanshghlouh direct
 * 4. Ila kan command wa7da → execute_single_command()
 * 5. Ila kan pipes → execute_pipe()
 * 
 * Decision tree:
 * - No command/args → just redirections
 * - Builtin + no pipes → direct execution
 * - Single command → fork + exec
 * - Multiple commands → pipe execution
 */
void	executer(t_data *data, char **envp)
{
	(void)envp;
	
	// ila makansh 3ndna command wla args
	if (!data->cmd || !data->cmd->args)
		return ;
		
	// ila makansh 3ndna args, ghi kandiro redirections
	if (!data->cmd->args[0])
	{
		if (setup_redirections(data->cmd) < 0)
			data->exit_status = 1;
		return ;
	}
	
	// ila kan builtin o makansh pipes
	if (is_builtin(data->cmd->args[0]) && !data->cmd->next)
	{
		data->exit_status = handle_builtin(data);
		return ;
	}
	
	// ila kan command wa7da mn ghir pipes
	if (!data->cmd->next)
	{
		data->exit_status = execute_single_command(data);
		return ;
	}
	
	// ila kan 3ndna pipes
	execute_pipe(data);
}
