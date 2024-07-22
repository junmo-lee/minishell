/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pipex_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:57:08 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/22 20:44:43 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_local_path(t_vars *vars, t_cmd *cmd)
{
	char	*temp;

	if (access(cmd->cmd_name, X_OK) == 0)
	{
		cmd->cmd_path = cmd->cmd_name;
		return (1);
	}
	else
	{
		vars++;
		temp = ft_substr(cmd->cmd_argv, 2, ft_strlen(cmd->cmd_argv) - 2);
		if (access(temp, X_OK) == 0)
		{
			cmd->cmd_path = temp;
			return (1);
		}
		else
		{
			ft_memset(temp, 0, ft_strlen(temp));
			free(temp);
		}
	}
	return (0);
}

void	child(t_vars *vars, t_cmd *cmd)
{
	if (dup2(vars->prev_read, STDIN_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(vars->prev_read);
	if (dup2(vars->next_write, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(vars->next_write);
	close(vars->pipe_fd[0]);
	if (check_local_path(vars, cmd) == 0)
		check_cmd_access(vars, cmd);
	if (execve(cmd->cmd_path, cmd->args, cmd->envp) == -1)
		exit(EXIT_FAILURE);
}

void	close_fd_main(t_vars *vars, int count)
{
	close(vars->prev_read);
	close(vars->next_write);
	if (count != vars->cmd_len - 1)
	{
		vars->prev_read = dup(vars->pipe_fd[0]);
		close(vars->pipe_fd[0]);
	}
}

void	check_cmd_access(t_vars *vars, t_cmd *cmd)
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
			cmd->cant_execute = 1;
			if (access(temp, X_OK) == 0)
			{
				cmd->cmd_path = temp;
				return ;
			}
		}
		free(temp);
		i++;
	}
	if (cmd->cant_execute)
		write_stderr_exit("Permission denied: ", cmd->cmd_name, 126);
	else
		write_stderr_exit("Command not found: ", cmd->cmd_name, 127);
}

// void	do_pipex_bonus(t_vars *vars, t_cmd *cmd)
// {
// 	int		count;
// 	pid_t	fork_ret;

// 	vars->prev_read = dup(vars->file1_read_fd);
// 	close(vars->file1_read_fd);
// 	count = 0;
// 	while (count < vars->cmd_len)
// 	{
// 		cmd_init(vars, (cmd + count), count, \
// 			vars->argv[count + 2 + vars->is_here_doc]);
// 		fork_ret = fork();
// 		if (fork_ret == -1)
// 			exit(EXIT_FAILURE);
// 		else if (fork_ret == 0)
// 			child(vars, (cmd + count));
// 		else
// 		{
// 			(cmd + count)->pid = fork_ret;
// 			close_fd_main(vars, count);
// 		}
// 		count++;
// 	}
// }
