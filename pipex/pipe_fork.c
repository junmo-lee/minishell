/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:07:30 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/06 21:13:38 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_fork_init(t_vars *vars, t_cmd *cmd, int count)
{
	(cmd + count)->envp = vars->envp;
	if ((cmd + count)->redirection_out != -1)
	{
		dup2((cmd + count)->redirection_out, vars->next_write);
		close((cmd + count)->redirection_out);
		// dup_close(&(vars->next_write), &((cmd + count)->redirection_out));
	}
	else
	{
		if (count != vars->cmd_len - 1)
		{
			if (pipe(vars->pipe_fd) == -1)
				exit(EXIT_FAILURE);
			dup2(vars->pipe_fd[1], vars->next_write);
			close(vars->pipe_fd[1]);
			// dup_close(&(vars->next_write), &(vars->pipe_fd[1]));
		}
		else
			vars->next_write = dup(STDOUT_FILENO);
	}
}

void	pipe_fork(t_status *status, t_vars *vars, t_cmd *cmd, int count)
{
	pid_t	fork_ret;

	// pipe_fork_init(vars, cmd, count);
	(cmd + count)->envp = vars->envp;
	fprintf(stderr, "rin : %d rout : %d\n", (cmd + count)->redirection_in, (cmd + count)->redirection_out);
	if (count != vars->cmd_len - 1)
	{
		if (pipe(vars->pipe_fd) == -1)
				exit(EXIT_FAILURE);
	}
	fork_ret = fork();
	if (fork_ret == -1)
		exit(EXIT_FAILURE);
	else if (fork_ret == 0)
	{
		child(vars, (cmd + count), status, count);
	}
	else
	{
		(cmd + count)->pid = fork_ret;
		if ((cmd + count)->redirection_in != -1)
			close((cmd + count)->redirection_in);
		if ((cmd + count)->redirection_out != -1)
			close((cmd + count)->redirection_out);
		if (count != vars->cmd_len - 1)
		{
			close(vars->pipe_fd[1]);
			dup2(vars->pipe_fd[0], vars->prev_read);
			close(vars->pipe_fd[0]);
			// dup_close(&(vars->prev_read), &(vars->pipe_fd[0]));
		}
		check_fd("main");
	}
}
