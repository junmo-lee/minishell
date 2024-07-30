#include "pipex_bonus.h"

// 빌트인이면 여기서 exit 되도록
int pipe_built_in(t_vars *vars, t_cmd *cmd, t_status *status)
{
	if (ft_strncmp(cmd->cmd_name, "echo", sizeof("echo")) == 0)
		pipe_echo(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "cd", sizeof("cd")) == 0)
		pipe_cd(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "pwd", sizeof("pwd")) == 0)
		pipe_pwd(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "export", sizeof("export")) == 0)
		pipe_export(vars, cmd, status);
	else if (ft_strncmp(cmd->cmd_name, "unset", sizeof("unset")) == 0)
		pipe_unset(vars, cmd, status);
	else if (ft_strncmp(cmd->cmd_name, "env", sizeof("env")) == 0)
		pipe_env(vars, cmd, status);
	else if (ft_strncmp(cmd->cmd_name, "exit", sizeof("exit")) == 0)
		pipe_exit(vars, cmd);
	return (0);
}


int	check_cmd_local(t_cmd *cmd)
{
	if (access(cmd->cmd_name, F_OK) == 0)
	{
		if (access(cmd->cmd_name, X_OK) == 0)
		{
			cmd->cmd_path = cmd->cmd_name;
			return (1);
		}
		else
		{
			perror(cmd->cmd_name);
			exit(EACCES_EXIT_CODE);
		}
	}
	perror(cmd->cmd_name);
	exit(COMMAND_NOT_FOUND);
	return (1);
}


int	check_cmd_path(t_status *status, t_vars *vars, t_cmd *cmd)
{
	char	*temp;
	int		i;

	i = -1;
	while (vars->path[++i] != NULL)
	{
		temp = path_join(vars->path[i], cmd->cmd_name);
		if (temp == NULL)
			exit(EXIT_FAILURE);
		if (access(temp, F_OK) == 0)
		{
			cmd->is_exist = 1;
			if (access(temp, X_OK) == 0)
			{
				cmd->cmd_path = temp;
				return (1);
			}
		}
		free(temp);
	}
	if (cmd->is_exist == 1)
		print_full_path(status->pwd, cmd->cmd_name);
	else
		write_stderr_exit(cmd->cmd_name, ": command not found", COMMAND_NOT_FOUND);
	return (1);
}

int	check_cmd(t_status *status, t_vars *vars, t_cmd *cmd)
{
	int	i;

	if (cmd->cmd_name == NULL)
		return (0);
	cmd->is_exist = 0;
	i = 0;
	while (cmd->cmd_name[i] != '\0')
	{
		if (cmd->cmd_name[i] == '/')
			return (check_cmd_local(cmd));
		i++;
	}
	return (check_cmd_path(status, vars, cmd));
}