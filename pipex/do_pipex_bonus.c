/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pipex_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:57:08 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/30 20:05:34 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	check_local_path(t_cmd *cmd)
{
	if (access(cmd->cmd_name, X_OK) == 0)
	{
		cmd->cmd_path = cmd->cmd_name;
		return (1);
	}
	return (0);
}

void	child(t_vars *vars, t_cmd *cmd, t_status *status)
{
	int	dup2_ret;

	if (cmd->redirection_fail == 1)
		exit(EXIT_FAILURE);
	//check_fd("child start");
	if (cmd->redirection_in != -1)
	{
		dup2(cmd->redirection_in, STDIN_FILENO);
		close(cmd->redirection_in);
		close(vars->prev_read);
	}
	else
	{
		dup2_ret = dup2(vars->prev_read, STDIN_FILENO);
		if (dup2_ret == -1)
			exit(EXIT_FAILURE);
		if (dup2_ret != STDIN_FILENO)
			close(vars->prev_read);
	}
	dup2_ret = dup2(vars->next_write, STDOUT_FILENO);
	if (dup2_ret == -1)
		exit(EXIT_FAILURE);
	if (dup2_ret != STDOUT_FILENO)
		close(vars->next_write);
	// check_fd("cmd execve");
	pipe_built_in(vars, cmd, status);
		// 빌트인이 아닐때 local -> path 순서로
	check_cmd(status, vars, cmd);
	if (execve(cmd->cmd_path, cmd->args, cmd->envp) == -1)
		exit(EXIT_FAILURE);
	// //fprintf(stderr, "[%s]\n", cmd->cmd_path);
	// if(cmd->args != NULL)
	// 	for(int i=0;cmd->args[i]!=NULL;i++)
	// 		//fprintf(stderr, "[%s]\n", cmd->args[i]);
	exit(EXIT_SUCCESS);
}
