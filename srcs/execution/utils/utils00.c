#include "../../../includes/minishell.h"

/*
 * HNA KANQALBO F KOLL PATHS 3LA L COMMAND
 * 
 * Ashno katdir:
 * 1. Kandouro 3la koll path f paths array
 * 2. Koll path kanزيدو "/" o l command name
 * 3. Kanشofou ila l full path executable (access X_OK)
 * 4. Ila l9ina executable path → kanrj3ouh
 * 5. Ila maقدرناش nl9awha → NULL
 * 
 * Mathal: ila PATH="/bin:/usr/bin" o command="ls"
 * - Kanجرب "/bin/ls" 
 * - Ila makanx, kanجرب "/usr/bin/ls"
 * - Ila l9ina واحد executable → kanrj3ouh
 */
static char	*search_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

/*
 * HNA KANQALBO 3LA L PATH DYAL L COMMAND
 * 
 * Ashno katdir:
 * 1. Kanشofou ila 3ndna command o data o environment
 * 2. Kanاخدو PATH mn environment variables
 * 3. Kanقسمو PATH b ':' (colon) bash n7slo 3la array dyal paths
 * 4. Kanشghlo search_path() bash nقلبو f koll path
 * 5. Kannدفو l paths array
 * 
 * PATH variable format: "/bin:/usr/bin:/usr/local/bin"
 * B3d split: ["/bin", "/usr/bin", "/usr/local/bin"]
 * 
 * Ila makanx PATH → NULL (command manl9awهاش)
 */
char	*get_path(t_data *data, char *cmd)
{
	char	**paths;
	char	*env_path;
	char	*final_path;

	// ila makansh command wla data
	if (!cmd || !data || !data->env)
		return (NULL);
		
	// kanqalbo 3la PATH f environment
	env_path = get_env_value(data->env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	final_path = search_path(paths, cmd);
	free_array(paths);
	return (final_path);
}

/*
 * HNA KANاخدوU L FILENAME MN STRING
 * 
 * Ashno katdir:
 * 1. Kanتخطى l spaces mn l beginning
 * 2. Kanاخدو characters 7ta nوصلو l space
 * 3. Ila l filename surrounded b quotes → kanحذفوها
 * 4. Kanعملو malloc o kanنسخو l filename
 * 
 * Mathal:
 * - "  file.txt  " → "file.txt"
 * - "'quoted.txt'" → "quoted.txt"  
 * - "\"double.txt\"" → "double.txt"
 * 
 * Hadi استعملوها f redirections bash nستخرجو l filename
 */
static char	*extract_filename_content(char *files, int start, int end)
{
	char	*file;
	int		i;

	i = 0;
	file = malloc(end - start + 1);
	if (!file)
		return (NULL);
	while (start < end)
		file[i++] = files[start++];
	file[i] = '\0';
	return (file);
}

char	*get_file_name(char *files)
{
	int	start;
	int	end;

	if (!files)
		return (NULL);
	start = 0;
	while (files[start] && files[start] == ' ')
		start++;
	end = start;
	while (files[end] && files[end] != ' ')
		end++;
	if ((files[start] == '\'' && files[end - 1] == '\'') ||
		(files[start] == '\"' && files[end - 1] == '\"'))
	{
		start++;
		end--;
	}
	return (extract_filename_content(files, start, end));
}

/*
 * HNA KANشofوU ILA L COMMAND BUILTIN
 * 
 * Ashno katdir:
 * - Kanقارنو l command name m3a list dyal builtins
 * - Builtin commands: cd, echo, exit, pwd, env, export, unset
 * - Kanرج3و 1 ila builtin, 0 ila external command
 * 
 * Limada builtins مختلفين:
 * - Kaيتنفذو f shell process نفسو (makat9ouch fork)
 * - Y9dro يغيرو shell state (cd يغير directory)
 * - Environment variables (export/unset يغيرو environment)
 */
int	is_builtin(char *cmd)
{
	// ila makansh command
	if (!cmd)
		return (0);
	// kanshofou ila kayn f l builtins
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") ||
		!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "pwd") ||
		!ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset"));
}

/*
 * HNA KANشغلوU L BUILTIN COMMAND
 * 
 * Ashno katdir:
 * - Kanشofou ashno l builtin command
 * - Kanشغلو l function المناسبة m3a args
 * - Kanرج3و exit status
 * 
 * Koll builtin 3ndo function خاصة بيه:
 * - cd → ft_cd() (يغير current directory)
 * - echo → ft_echo() (يطبع arguments)
 * - exit → ft_exit() (يخرج mn shell)
 * - pwd → ft_pwd() (يطبع current directory)
 * - env → ft_env() (يطبع environment)
 * - export → ft_export() (يضيف environment variables)
 * - unset → ft_unset() (يحذف environment variables)
 */
int	execute_builtin(t_data *data)
{
	char	*cmd = data->cmd->args[0];

	// kanshofou ashno l builtin o kanshghlouh
	if (!ft_strcmp(cmd, "cd"))
		return ft_cd(data, data->cmd->args);
	if (!ft_strcmp(cmd, "echo"))
		return ft_echo(data->cmd->args);
	if (!ft_strcmp(cmd, "exit"))
		return (ft_exit(data, data->cmd->args));
	if (!ft_strcmp(cmd, "pwd"))
		return (ft_pwd(data));
	if (!ft_strcmp(cmd, "env"))
		return (ft_env(data, data->cmd->args));
	if (!ft_strcmp(cmd, "export"))
		return (ft_export(data, data->cmd->args));
	if (!ft_strcmp(cmd, "unset"))
		return (ft_unset(data, data->cmd->args));
	return (0);
}
