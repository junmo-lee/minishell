/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choolee <choolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 18:53:05 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/23 19:40:25 by choolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

// 디버그용
#include <stdio.h>

int	get_exit_status(int status)
{
	return (((*(int *)&(status)) >> 8) & 0x000000ff);
}

void	wait_processes(t_vars *vars, t_cmd *cmd)
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
				//free_cmds((cmd + count));
			}
			count++;
		}
	}
}

int	main_return(t_vars *vars, t_cmd *cmd)
{
	wait_processes(vars, cmd);
	if (vars->is_here_doc == 1)
		unlink(".here_doc");
	free_strs(vars->path, EXIT_SUCCESS);
	return (get_exit_status((cmd + (vars->cmd_len - 1))->status));
}

void	bonus_get_fds(t_vars *vars)
{
	if (ft_strncmp(vars->argv[1], "here_doc", ft_strlen(vars->argv[1])) == 0 \
		&& ft_strlen(vars->argv[1]) == ft_strlen("here_doc"))
	{
		vars->is_here_doc = 1;
		unlink(".here_doc");
		write_file(&(vars->here_doc_fd), ".here_doc", \
			O_WRONLY | O_CREAT | O_TRUNC);
		write_here_doc(vars);
		read_file(&(vars->file1_read_fd), ".here_doc", O_RDONLY);
		write_file(&(vars->file2_write_fd), vars->argv[vars->argc - 1], \
			O_WRONLY | O_CREAT | O_APPEND);
	}
	else
	{
		vars->is_here_doc = 0;
		read_file(&(vars->file1_read_fd), vars->argv[1], O_RDONLY);
		write_file(&(vars->file2_write_fd), vars->argv[vars->argc - 1], \
			O_WRONLY | O_CREAT | O_TRUNC);
	}
}

int	run_cmd_tree(t_status *status, t_vars *vars, t_parsed_tree *tree)
{
	t_cmd 			cmd[1024]; // 나중에 연결리스트 형태로 변경?
	t_parsed_tree	*current_node;
	t_parser_list	*parser_node;
	int				index;
	int				arg_index;

	int				save_stdin;
	int				save_stdout;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);

	// vars : main에서 argc, argv, envp, path 를 받아옴

	// 리다이엑션은 나중에 고려
	// bonus_get_fds(&vars);

	vars->cmd_len = tree->cmd_len;
	// cmd 로 바꾸는 과정(간단하게)
	// 첫번째 string을 프로그램명이라고 가정
	// parsed_tree
	// | parser_list 0 -> cmd[0]
	// | parser_list 1 -> cmd[1]
	// | parser_list 2 -> cmd[2]

	ft_memset(cmd, 0, sizeof(cmd));
	current_node = tree;
	index = 0; // cmd 0, cmd 1

	while (current_node != NULL)
	{
		fprintf(stderr, "arg_len : %d\n", current_node->arg_len);
		(cmd + index)->args = malloc(sizeof(char *) * (current_node->arg_len + 1));
		parser_node = current_node->cmd_list_head;
		arg_index = 0;
		while (parser_node != NULL)
		{
			if ((cmd + index)->cmd_name == NULL)
			{
				(cmd + index)->cmd_name = parser_node->token;
				fprintf(stderr, "cmd_name : [%s]\n", (cmd + index)->cmd_name);
			}
			(cmd + index)->args[arg_index] = parser_node->token;
			fprintf(stderr, "[%s]\n", (cmd + index)->args[arg_index]);
			arg_index++;
			parser_node = parser_node->next;
		}
		(cmd + index)->args[arg_index] = NULL;
		current_node = current_node->next;
		index++;
	}

	check_fd("main");
	int		count;
	pid_t	fork_ret;

	// 리다이엑션이 들어오면 stdin 이 아니라 file1_read_fd로
	vars->prev_read = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	count = 0;
	while (count < vars->cmd_len)
	{
		/*
		//cmd_init(vars, (cmd + count), count, \
			vars->argv[count + 2 + vars->is_here_doc]);
		*/
		(cmd + count)->envp = vars->envp;

		//parse_name_args(cmd, cmd_argv);
		if (count != vars->cmd_len - 1)
		{
			if (pipe(vars->pipe_fd) == -1)
				exit(EXIT_FAILURE);
			vars->next_write = dup(vars->pipe_fd[1]);
			close(vars->pipe_fd[1]);
		}
		else
		{
			vars->next_write = dup(STDOUT_FILENO);
			//vars->next_write = dup(vars->file2_write_fd);
			close(STDOUT_FILENO);
		}
		fork_ret = fork();
		if (fork_ret == -1)
			exit(EXIT_FAILURE);
		else if (fork_ret == 0)
			child(vars, (cmd + count));
		else
		{
			(cmd + count)->pid = fork_ret;
			//stdin, stdout 이 닫히면 안되서 일단 pass
			close_fd_main(vars, count);
		}
		count++;
	}
	status->exit_status = main_return(vars, cmd);
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	return (status->exit_status);
	//return (0);
}