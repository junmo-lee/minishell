#include "../parser.h"

int	wait_processes(t_vars *vars, t_cmd *cmd)
{
	int	count;
	int	exit_count;
	int	ret;

	exit_count = 0;
	while (exit_count < vars->cmd_len)
	{

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
				exit_count++;
				// 메모리는 나중에
				// 일단 위에서 할당한 args 만 해제
				if ((cmd + count)->args != NULL)
					free((cmd + count)->args);
				//free_cmds((cmd + count));
			}
			count++;
		}
	}
	if (g_signal == SIGINT)
		return (SIGINT_EXIT_CODE);
	else if (g_signal == SIGQUIT)
		return (SIGQUIT_EXIT_CODE);
	else
		return (0);
}

int	run_cmd_tree(t_status *status, t_parsed_tree *tree)
{
	t_cmd 			cmd[OPEN_MAX];
	t_vars			*vars;

	vars = status->one_line;
	vars->pwd = status->pwd;
	vars->cmd_len = tree->cmd_len;
	vars->is_here_doc = 0;

	ft_memset(cmd, 0, sizeof(cmd));
	if (tree->cmd_list_head == NULL)
		return (EXIT_SUCCESS);
	if (tree_parse(tree, vars, cmd) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	int		count;

	vars->prev_read = dup(STDIN_FILENO);
	count = 0;
	while (count < vars->cmd_len)
	{
		pipe_fork(status, vars, cmd, count);
		count++;
	}

	status->exit_status = wait_processes(vars, cmd);
	if (status->exit_status == 0)
		status->exit_status = get_exit_status((cmd + (vars->cmd_len - 1))->status);
	// check_fd("main");
	return (status->exit_status);
	//return (0);
}