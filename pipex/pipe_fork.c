#include "../minishell.h"

void	pipe_fork_init(t_vars *vars, t_cmd *cmd, int count)
{
	(cmd + count)->envp = vars->envp;
	if (count != vars->cmd_len - 1)
	{
		if (pipe(vars->pipe_fd) == -1)
			exit(EXIT_FAILURE);
	}
	if ((cmd + count)->redirection_out != -1)
		dup_close(&(vars->next_write), &((cmd + count)->redirection_out));
	else
	{
		if (count != vars->cmd_len - 1)
			dup_close(&(vars->next_write), &(vars->pipe_fd[1]));
		else
			vars->next_write = dup(STDOUT_FILENO);
	}
}

void	pipe_fork(t_status *status, t_vars *vars, t_cmd *cmd, int count)
{
	pid_t	fork_ret;

	pipe_fork_init(vars, cmd, count);
	fork_ret = fork();
	if (fork_ret == -1)
		exit(EXIT_FAILURE);
	else if (fork_ret == 0)
	{
		if (count != vars->cmd_len - 1)
			close(vars->pipe_fd[0]);
		child(vars, (cmd + count), status);
	}
	else
	{
		(cmd + count)->pid = fork_ret;
		close(vars->prev_read);
		close(vars->next_write);
		if (cmd->redirection_in != -1)
			close(cmd->redirection_in);
		if (count != vars->cmd_len - 1)
			dup_close(&(vars->prev_read), &(vars->pipe_fd[0]));
	}
}
