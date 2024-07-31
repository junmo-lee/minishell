#include "../parser.h"

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

void	print_command_not_found(char *name)
{
	write_stderr_exit(name, ": command not found", COMMAND_NOT_FOUND);
}

void	print_full_path(char *pwd, char *str)
{
	char	buf[PATH_MAX + 1];
	char	**temp_list;
	int		depth;
	int		len;
	int		i;

	temp_list = parse_dir(pwd, str);
	depth = strs_len(temp_list);
	ft_memset(buf, 0, ft_strlen(buf));
	i = 0;
	buf[0] = '/';
	while (i < depth)
	{
		len = ft_strlen(buf);
		buf[len] = '/';
		buf[len + 1] = '\0';
		ft_strlcat(buf, temp_list[i], \
			ft_strlen(buf) + ft_strlen(temp_list[i]) + 1);
		i++;
	}
	write(STDERR_FILENO, buf, ft_strlen(buf));
	free_strs(temp_list, EXIT_SUCCESS);
	write_stderr_exit(NULL, ": Permission denied", EACCES_EXIT_CODE);
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
		print_command_not_found(cmd->cmd_name);
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
