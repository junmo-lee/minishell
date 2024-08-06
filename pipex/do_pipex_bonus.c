/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pipex_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:57:08 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/06 19:04:45 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_local_path(t_cmd *cmd)
{
	if (access(cmd->cmd_name, X_OK) == 0)
	{
		cmd->cmd_path = cmd->cmd_name;
		return (1);
	}
	return (0);
}

void	child_init_fd(t_vars *vars, t_cmd *cmd, int count)
{
	int	dup2_ret;



	if (cmd->redirection_fail == 1)
	{
		if (count != vars->cmd_len - 1)
			close(vars->pipe_fd[1]);
		exit(EXIT_FAILURE);
	}

	if (cmd->redirection_in != -1)
	{
		dup2_ret = dup2(cmd->redirection_in, STDIN_FILENO);
		close(cmd->redirection_in);
		close(vars->prev_read);
	}
	else
	{
		dup2_ret = dup2(vars->prev_read, STDIN_FILENO);
		close(vars->prev_read);
	}


	if (cmd->redirection_out != -1)
	{
		dup2_ret = dup2(cmd->redirection_out, STDOUT_FILENO);
		close(cmd->redirection_out);
	}
	else
	{
		if (count != vars->cmd_len - 1)
			dup2_ret = dup2(vars->pipe_fd[1], STDOUT_FILENO);

	}

	if (count != vars->cmd_len - 1)
	{
		close(vars->pipe_fd[0]);
		close(vars->pipe_fd[1]);
	}
}

void	child(t_vars *vars, t_cmd *cmd, t_status *status, int count)
{
	child_init_fd(vars, cmd, count);
	if (cmd->cmd_name == NULL)
		exit(EXIT_SUCCESS);
	pipe_built_in(vars, cmd, status);
	check_cmd(status, vars, cmd);
	check_fd(cmd->cmd_name);
	if (execve(cmd->cmd_path, cmd->args, cmd->envp) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
