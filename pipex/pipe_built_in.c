#include "../minishell.h"

int	pipe_built_in(t_vars *vars, t_cmd *cmd, t_status *status)
{
	if (ft_strncmp(cmd->cmd_name, "echo", sizeof("echo")) == 0)
	{
		if (cmd->args[1] == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			exit(EXIT_SUCCESS);
		}
		else
			pipe_echo(vars, cmd);
	}
	else if (ft_strcmp(cmd->cmd_name, "cd") == 0)
		pipe_cd(status, cmd);
	else if (ft_strcmp(cmd->cmd_name, "pwd") == 0)
		pipe_pwd(vars, cmd);
	else if (ft_strcmp(cmd->cmd_name, "export") == 0)
		pipe_export(cmd, status);
	else if (ft_strcmp(cmd->cmd_name, "unset") == 0)
		pipe_unset(vars, cmd, status);
	else if (ft_strcmp(cmd->cmd_name, "env") == 0)
		pipe_env(vars, cmd, status);
	else if (ft_strcmp(cmd->cmd_name, "exit") == 0)
		pipe_exit(vars, cmd);
	return (0);
}
