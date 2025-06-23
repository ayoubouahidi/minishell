#include "../../includes/minishell.h"

/*
 * HAD L FUNCTION KATSED L FILE DESCRIPTOR ILA KAN M7LOUL
 * 
 * Ashno katdir:
 * - Katshof ila l fd >= 0 (ya3ni valid)
 * - Ila kan valid katsedo b close()
 * - Hadi 7it mn l7san nshofou 9bal ma nsedou
 * 
 * Limada n7tajouha:
 * - Bash maykounsh 3ndna leaks f file descriptors
 * - O bash man9lbloush errors ila 7awlna nsed fd invalid
 */
static void safe_close(int fd)
{
    if (fd >= 0)
        close(fd);
}

/*
 * HNA KAN9AD L PIPE O KANDIR FORK BACH NKHLQ9O PROCESS JDID
 * 
 * Ashno katdir:
 * 1. Ila kan 3ndna command jaya (cmd->next), kan9ad pipe jdid
 * 2. Kandir fork() bash nkhlq9o child process
 * 3. Kanrj3o l PID dyal l child
 * 
 * L pipe kaykhdem b joj taraf:
 * - fd[0]: read end (bash n9raw mnha)
 * - fd[1]: write end (bash nktbou fiha)
 * 
 * Fork kaydir:
 * - Parent process: kaybd9a f l execution normal
 * - Child process: kaymshi y3mel l command
 */
static pid_t create_pipe_and_fork(int *fd, t_command *cmd)
{
    pid_t pid;

    // ila kan 3ndna command okhra, ghadi n7tajou pipe
    if (cmd->next && pipe(fd) == -1)
    {
        perror("minishell: pipe");
        exit(EXIT_FAILURE);
    }
    // daba ghnkhlq9o process jdid
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

/*
 * HAD L FUNCTION KATSITR 3LA L INPUT O L OUTPUT DYAL L PROCESS SGHIR
 * 
 * Ashno katdir:
 * 1. Ila kan 3ndna input mn pipe 9dima (pre_fd):
 *    - Kanrbt9ouh b stdin bash l command t9ra mnha
 * 
 * 2. Ila kan 3ndna command jaya (cmd->next):
 *    - Kansdou l read end dyal l pipe (fd[0]) 7it mankra9sh fiha
 *    - Kanrbt9ou l write end (fd[1]) b stdout bash l command tkteb fiha
 * 
 * Mathal: f "ls | grep .c"
 * - ls: stdout → pipe[1] (katkteb l files f pipe)
 * - grep: stdin ← pipe[0] (kat9ra l files mn pipe)
 */
static void setup_child(int pre_fd, int *fd, t_command *cmd)
{
    // ila kan 3ndna input mn pipe 9dima
    if (pre_fd != -1)
    {
        dup2(pre_fd, STDIN_FILENO);
        safe_close(pre_fd);
    }
    // ila kan 3ndna command okhra, ghn7tajou nrbt9o bl pipe
    if (cmd->next)
    {
        safe_close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        safe_close(fd[1]);
    }
}

/*
 * HNA KANSHGHLO L COMMAND F L PROCESS SGHIR
 * 
 * L steps:
 * 1. Kandiro redirections lawla (>, <, >>)
 * 2. Kanshofou ila builtin (cd, echo, exit...):
 *    - Ila kan builtin kanshghlouh hna direct
 * 3. Ila external command:
 *    - Kanqalbo 3la l path dyalo f environment
 *    - Kan7awlo nshghlouh b execve()
 * 
 * Exit codes:
 * - 0: success
 * - 1: redirections fshlaw
 * - 126: execve fshel (permission denied)
 * - 127: command not found
 */
static void execute_child(t_data *data, t_command *cmd)
{
	char *path;
	char **envp;

	if (setup_redirections(cmd) < 0)
		exit(1);
		
	if (cmd->args && is_builtin(cmd->args[0]))
	{
		execute_builtin(data);
		exit(data->exit_status);
	}
	
	if (!cmd->args || !cmd->args[0])
		exit(0);
		
	path = get_path(data, cmd->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	envp = env_to_array(data->env);
	execve(path, cmd->args, envp);
	perror("minishell: execve");
	free(path);
	free_array(envp);
	exit(126);
}

/*
 * HNA KANNDFO L RESOURCES DYAL L PARENT PROCESS
 * 
 * Ashno katdir:
 * 1. Kansed l pre_fd ila kan m7loul (ma3adnach n7tajou)
 * 2. Ila kan 3ndna command jaya:
 *    - Kansed l write end dyal l pipe 7it parent mayktebuach fiha
 *    - Kan7fdo l read end bash nwslou l command jaya
 * 
 * Hadi mohima 7it:
 * - Ila mansdouch l pipes, l commands matkmllch (hanging)
 * - Koll process khaso y3ref imta l pipe tsd
 */
static void parent_cleanup(int *pre_fd, int *fd, t_command **cmd)
{
    if (*pre_fd != -1)
        safe_close(*pre_fd);
    if ((*cmd)->next)
    {
        safe_close(fd[1]);
        *pre_fd = fd[0];
    }
}

/*
 * HADI HIYA L FUNCTION RAYSIYA DYAL TSHGHIL L PIPE
 * 
 * Kifash katkhdem:
 * 1. Kanbdaw b tahyi2 l variables (pre_fd, counter, current command)
 * 2. Kandouro 3la koll command f linked list:
 *    - Kan9ad pipe jdid
 *    - Kandir fork() bash nkhlq9o child process
 *    - F child: kansetup l I/O o kanshghel l command
 *    - F parent: kanndaf o kanmshi l command jaya
 * 3. Kantsennaw koll l children ykmlo b waitpid()
 * 4. Kanakhdo l exit status mn akhir command
 * 
 * Mathal "ls | grep .c | wc -l":
 * - Command 1 (ls): stdout → pipe1
 * - Command 2 (grep): stdin ← pipe1, stdout → pipe2
 * - Command 3 (wc): stdin ← pipe2, stdout → terminal
 */
void execute_pipe(t_data *data)
{
	t_pipe	p;
	int		status;
	int		j;

	// kanbdaw b tahyi2 l variables
	p.pre_fd = -1;
	p.i = 0;
	p.cur = data->cmd;
	
	// kandouro 3la koll command o kanshghlouha
	while (p.cur)
	{
		p.pid = create_pipe_and_fork(p.fd, p.cur);
		p.pids[p.i++] = p.pid;
		// ila kna f l process sghir
		if (p.pid == 0)
			setup_child(p.pre_fd, p.fd, p.cur), execute_child(data, p.cur);
		else
			// ila kna f l parent process
			parent_cleanup(&p.pre_fd, p.fd, &p.cur), p.cur = p.cur->next;
	}
	
	// kantsennaw koll l processes ykmlo
	j = 0;
	while (j < p.i)
	{
		waitpid(p.pids[j], &status, 0);
		// kanakhdo l exit status dyal akher command
		if (WIFEXITED(status) && j == p.i - 1)
			data->exit_status = WEXITSTATUS(status);
		j++;
	}
}
