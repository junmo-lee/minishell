/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_built_in4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:05:48 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/05 17:57:55 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_env_print_error(char *str)
{
	ft_putstr_fd("env: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

void	pipe_env(t_cmd *cmd, t_status *status)
{
	t_envp_list	*current_node;

	current_node = status->env_list;
	if (cmd->args[1] != NULL)
		return ;
	while (current_node != NULL)
	{
		if (current_node->value == NULL)
		{
			current_node = current_node->next;
			continue ;
		}
		write(1, current_node->key, ft_strlen(current_node->key));
		write(1, "=", 1);
		write(1, current_node->value, ft_strlen(current_node->value));
		write(1, "\n", 1);
		current_node = current_node->next;
	}
	exit(EXIT_SUCCESS);
}

void	pipe_cd_print_error(char *str, char *error_msg)
{
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	pipe_cd_get_home(t_status *status, t_cmd *cmd)
{
	char	*env_home;

	env_home = ft_getenv("HOME", status->env_list);
	if (env_home == NULL)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	cmd->args[1] = env_home;
}

void	pipe_cd(t_status *status, t_cmd *cmd)
{
	struct stat	dir_stat;

	if (cmd->args[1] == NULL)
		pipe_cd_get_home(status, cmd);
	if (stat(cmd->args[1], &dir_stat) == 0)
	{
		if (!S_ISLNK(dir_stat.st_mode))
		{
			if (!S_ISDIR(dir_stat.st_mode))
				pipe_cd_print_error(cmd->args[1], ": Not a directory\n");
		}
		if (access(cmd->args[1], F_OK) != 0)
			pipe_cd_print_error(cmd->args[1], ": No such file or directory\n");
		else if (access(cmd->args[1], X_OK) != 0)
			pipe_cd_print_error(cmd->args[1], ": Permission denied\n");
		else
			exit(EXIT_SUCCESS);
	}
	else
		pipe_cd_print_error(cmd->args[1], ": No such file or directory\n");
}
