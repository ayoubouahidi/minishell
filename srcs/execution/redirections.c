#include "../../includes/minishell.h"

/*
 * CHRA7 DYAL REDIRECTIONS - KIFASH KATKHDM
 * 
 * Redirections huma bash nbdlo mnin l command tat9ra o mnin katkteb:
 * 
 * 1. INPUT REDIRECTION (<):
 *    - Normal: command tat9ra mn keyboard (stdin)
 *    - M3a <: command tat9ra mn file
 *    - Mathal: "cat < file.txt" → cat tat9ra mn file.txt badal mn keyboard
 * 
 * 2. OUTPUT REDIRECTION (>):
 *    - Normal: command katkteb f terminal (stdout) 
 *    - M3a >: command katkteb f file (kaym7o l file)
 *    - Mathal: "ls > output.txt" → ls katkteb f output.txt
 * 
 * 3. APPEND REDIRECTION (>>):
 *    - Bhal > ولكن kayزيد f l file mn ghir ma ym7ih
 *    - Mathal: "echo hello >> file.txt" → kayزيد "hello" f akhir l file
 * 
 * KIFASH KATKHDM TECHNICALLY:
 * - File descriptors: stdin=0, stdout=1, stderr=2
 * - dup2() katبدل منين l fd katشير
 * - Mathal: dup2(file_fd, STDOUT_FILENO) → stdout يولي يشير ل file
 */

/*
 * HNA KANSHOFOU ILA L STRING FARGA WLA FIH GHI SPACES
 * 
 * Ashno katdir:
 * - Ila l string NULL → true (farga)
 * - Kandouro 3la koll character f string
 * - Ila l9ina character makynchx space/tab/newline → false
 * - Ila koll characters spaces → true
 * 
 * Limada n7tajouha:
 * - F redirections, ila l filename farg → syntax error
 * - Mathal: "echo hello > $empty_var" → error
 * - Variable expansion y9dr y3ti string farg
 */
static int is_empty_or_whitespace(char *str)
{
	// ila makansh string
	if (!str)
		return (1);
	// kandouro 3la koll character
	while (*str)
	{
		// ila l9ina character makynchx space
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}

/*
 * HNA KANDIRO INPUT REDIRECTION
 * 
 * Ashno katdir:
 * 1. Kanshofou ila l filename farg → syntax error
 * 2. Kan7awlo nft7o l file b O_RDONLY (read only)
 * 3. Ila maft7x → error message
 * 4. Kanrbt9o l file b stdin b dup2()
 * 
 * B3d hadi, koll input li l command tat9ra kaytji mn l file
 * 
 * Mathal: "cat < input.txt"
 * - cat tat9ra mn input.txt badal mn keyboard
 */
static int handle_input_redirection(t_redirections *redir)
{
	int fd;
	
	// kanshofou ila l file farg
	if (is_empty_or_whitespace(redir->file))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	
	// kan7awlo nft7o l file
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(redir->file);
		return (-1);
	}
	
	// kanrbt9o l file b stdin
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * HNA KANDIRO OUTPUT REDIRECTION
 * 
 * L Process step by step:
 * 1. Kanشofou ila l filename فارغ → error
 * 2. Kan7ددو l flags:
 *    - O_WRONLY: write only (ghi nكتبو)
 *    - O_CREAT: create file ila makanx (نخلقو ila makaynx)
 *    - O_TRUNC (>): truncate file (nm7o l content)
 *    - O_APPEND (>>): append to file (nزيدو f l akhir)
 * 3. Kanft7o l file b open() m3a permissions 0644:
 *    - 0644 = owner: read+write, group: read, others: read
 * 4. dup2(fd, STDOUT_FILENO):
 *    - Kayخلي stdout يشير ل file بدال terminal
 *    - B3d hadi, koll printf/write على stdout kaymshi l file
 * 5. Kansد l file descriptor الأصلي (maخصناش b3d dup2)
 * 
 * Mathal practical:
 * - "echo hello > file.txt"
 * - قبل: stdout → terminal
 * - بعد redirection: stdout → file.txt  
 * - echo katkteb "hello" f file.txt بدال terminal
 */
static int handle_output_redirection(t_redirections *redir)
{
	int	fd;
	int	flags;

	if (is_empty_or_whitespace(redir->file))
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
		return (-1);
	}
	if (redir->type == TOKEN_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->file, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(redir->file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * HADI L FUNCTION KATDIR KOLL REDIRECTIONS
 * 
 * L Algorithm:
 * 1. Kandouro f linked list dyal redirections
 * 2. Koll redirection kantعاملو حسب type dyalha
 * 3. L ORDER مهم جداً:
 *    - "cmd < file1 < file2" → akhir واحد يغلب (file2)
 *    - "cmd > file1 > file2" → output يمشي ل file2
 * 4. Ila shi redirection فشلت → kanوقفو o kanرجعو error
 * 
 * Real examples:
 * - "cat < input.txt > output.txt":
 *   1. Setup input redirection: stdin ← input.txt
 *   2. Setup output redirection: stdout → output.txt  
 *   3. cat tat9ra mn input.txt o katkteb f output.txt
 * 
 * - "ls -l >> log.txt 2>&1":
 *   1. stdout → log.txt (append mode)
 *   2. stderr → stdout (2>&1 - هادي مش implemented هنا)
 * 
 * ERROR CASES:
 * - File makaynx for input → "No such file or directory"
 * - Permission denied for output → "Permission denied"  
 * - Empty filename → "ambiguous redirect"
 */
int setup_redirections(t_command *cmd)
{
	t_redirections *redir;

	// ila makansh command wla redirections
	if (!cmd || !cmd->redirections)
		return (0);
	
	// kandouro 3la koll redirection
	redir = cmd->redirections;
	while (redir)
	{
		// input redirection
		if (redir->type == TOKEN_REDIRECT_IN)
		{
			if (handle_input_redirection(redir) < 0)
				return (-1);
		}
		// output redirection aw append
		else if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_APPEND)
		{
			if (handle_output_redirection(redir) < 0)
				return (-1);
		}
		
		redir = redir->next;
	}
	return (0);
}


