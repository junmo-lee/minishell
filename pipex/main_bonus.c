#include "../parser.h"
#include <signal.h>
// 디버그용
#include <stdio.h>

void	stdin_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = SIGINT;
		write(STDOUT_FILENO, "^C\n", 4);
	}
	if (signo == SIGQUIT)
	{
		g_signal = SIGQUIT;
		write(STDOUT_FILENO, "^\\Quit: 3\n", 11);
	}
}

int	close_all_fd(t_vars *vars, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < vars->cmd_len)
	{
		if ((cmd + i)->redirection_in != -1)
			close((cmd + i)->redirection_in);
		if ((cmd + i)->redirection_out != -1)
			close((cmd + i)->redirection_out);
	}
	return (0);
}

int	get_exit_status(int status)
{
	return (((*(int *)&(status)) >> 8) & 0x000000ff);
}

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
	t_cmd 			cmd[OPEN_MAX]; // 나중에 연결리스트 형태로 변경?
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
	// while (current_node != NULL)
	// {
	// 	(cmd + index)->redirection_in = -1;
	// 	(cmd + index)->redirection_out = -1;
	// 	// //fprintf(stderr, "arg_len : %d\n", current_node->arg_len);
	// 	(cmd + index)->args = malloc(sizeof(char *) * (current_node->arg_len + 1));
	// 	parser_node = current_node->cmd_list_head;
	// 	arg_index = 0;
	// 	while (parser_node != NULL)
	// 	{
	// 		rl_done = 0;
	// 		signal(SIGINT, stdin_handler);
	// 		signal(SIGQUIT, stdin_handler);
	// 		if (parser_node->type == HERE_DOC)
	// 		{
	// 			if ((cmd + index)->redirection_in != -1)
	// 				close((cmd + index)->redirection_in);
	// 			parser_node = parser_node->next;
	// 			if (make_here_doc(vars, cmd + index, parser_node->token) == SIGINT)
	// 			{
	// 				free((cmd + index)->args);
	// 				free(vars->temp_here_doc);
	// 				return (EXIT_FAILURE);
	// 			}
	// 		}
	// 		else if (parser_node->type == REDIRECTION)
	// 		{
	// 			if ((cmd + index)->redirection_fail == 1)
	// 				break ;
	// 			if (parser_node->token[0] == '<')
	// 			{
	// 				parser_node = parser_node->next;
	// 				if ((cmd + index)->redirection_in != -1)
	// 					close((cmd + index)->redirection_in);
	// 				if (read_file(&((cmd + index)->redirection_in),parser_node->token, O_RDONLY))
	// 					(cmd + index)->redirection_fail = 1;
	// 			}
	// 			else if (parser_node->token[0] == '>')
	// 			{
	// 				if ((cmd + index)->redirection_out != -1)
	// 					close((cmd + index)->redirection_out);
	// 				if (parser_node->token[1] == '>')
	// 				{
	// 					parser_node = parser_node->next;
	// 					if (write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_APPEND))
	// 						(cmd + index)->redirection_fail = 1;
	// 				}
	// 				else
	// 				{
	// 					parser_node = parser_node->next;
	// 					if (write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_TRUNC))
	// 						(cmd + index)->redirection_fail = 1;
	// 				}
	// 			}
	// 		}
	// 		else
	// 		{
	// 			(cmd + index)->args[arg_index] = parser_node->token;
	// 			arg_index++;
	// 		}
	// 		parser_node = parser_node->next;
	// 	}
	// 	(cmd + index)->cmd_name = (cmd + index)->args[0];
	// 	(cmd + index)->args[arg_index] = NULL;
	// 	current_node = current_node->next;
	// 	index++;
	// }

	int		count;
	pid_t	fork_ret;

	vars->prev_read = dup(STDIN_FILENO);
	count = 0;
	while (count < vars->cmd_len)
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
			// check_fd("parant");
			(cmd + count)->pid = fork_ret;
			close(vars->prev_read);
			close(vars->next_write);
			if (cmd->redirection_in != -1)
				close(cmd->redirection_in);
			if (count != vars->cmd_len - 1)
				dup_close(&(vars->prev_read), &(vars->pipe_fd[0]));
		}
		count++;
	}

	status->exit_status = wait_processes(vars, cmd);
	if (status->exit_status == 0)
		status->exit_status = get_exit_status((cmd + (vars->cmd_len - 1))->status);
	// check_fd("main");
	return (status->exit_status);
	//return (0);
}