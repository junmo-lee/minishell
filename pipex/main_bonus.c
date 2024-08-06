/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:49:12 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/06 19:14:14 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_all_cmd(t_vars *vars, t_cmd *cmd, int *exit_count)
{
	int	ret;
	int	count;

	count = 0;
	while (count < vars->cmd_len)
	{
		ret = 0;
		if ((cmd + count)->is_end != 1)
			ret = waitpid((cmd + count)->pid, \
				&((cmd + count)->status), WNOHANG);
		if (ret == -1)
			exit (EXIT_FAILURE);
		else if (ret != 0)
		{
			(cmd + count)->is_end = 1;
			(*exit_count)++;
			if ((cmd + count)->args != NULL)
				free((cmd + count)->args);
		}
		count++;
	}
}

int	wait_processes(t_vars *vars, t_cmd *cmd)
{
	int	exit_count;

	exit_count = 0;
	while (exit_count < vars->cmd_len)
		check_all_cmd(vars, cmd, &exit_count);
	if (g_signal == SIGINT)
		return (SIGINT_EXIT_CODE);
	else if (g_signal == SIGQUIT)
		return (SIGQUIT_EXIT_CODE);
	else
		return (0);
}

void	run_cmd_tree_init(t_status *status, t_parsed_tree *tree, t_vars *vars)
{
	vars = status->one_line;
	vars->pwd = status->pwd;
	vars->cmd_len = tree->cmd_len;
	vars->is_here_doc = 0;
}

int	run_cmd_tree(t_status *status, t_parsed_tree *tree, t_vars *vars)
{
	t_cmd			cmd[OPEN_MAX];
	int				count;

	run_cmd_tree_init(status, tree, vars);
	ft_memset(cmd, 0, sizeof(cmd));
	if (tree->cmd_list_head == NULL)
		return (EXIT_SUCCESS);
	if (tree_parse(tree, vars, cmd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	check_fd("before forking");
	vars->prev_read = dup(STDIN_FILENO);
	count = 0;
	while (count < vars->cmd_len)
	{
		pipe_fork(status, vars, cmd, count);
		count++;
	}
	close(vars->prev_read);
	status->exit_status = wait_processes(vars, cmd);
	if (status->exit_status == 0)
		status->exit_status = \
			get_exit_status((cmd + (vars->cmd_len - 1))->status);
	return (status->exit_status);
}
