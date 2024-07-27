/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 14:59:42 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/27 16:59:37 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// 빌트인이면 여기서 exit 되도록
int pipe_built_in(t_vars *vars, t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd_name, "echo", sizeof("echo")) == 0)
		pipe_echo(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "cd", sizeof("cd")) == 0)
		pipe_cd(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "pwd", sizeof("pwd")) == 0)
		pipe_pwd(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "export", sizeof("export")) == 0)
		pipe_export(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "unset", sizeof("unset")) == 0)
		pipe_unset(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "env", sizeof("env")) == 0)
		pipe_env(vars, cmd);
	else if (ft_strncmp(cmd->cmd_name, "exit", sizeof("exit")) == 0)
		pipe_exit(vars, cmd);
	return (0);
}

int	check_cmd(t_vars *vars, t_cmd *cmd)
{
	if (access(cmd->cmd_name, F_OK) == 0)
	{
		cmd->is_exist = 1;
		if (access(cmd->cmd_name, F_OK) == X_OK)
		{
			cmd->cmd_path = cmd->cmd_name;
			return (1);
		}
	}
	return (check_cmd_path(vars, cmd));
}

int	check_cmd_path(t_vars *vars, t_cmd *cmd)
{
	char	*temp;
	int		i;

	i = 0;
	while (vars->path[i] != NULL)
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
		i++;
	}
	return (0);
}