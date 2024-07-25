/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 18:53:05 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/25 16:43:41 by junmlee          ###   ########.fr       */
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
				// 일단 위에서 할당한 args 만 해제
				if ((cmd + count)->args != NULL)
					free((cmd + count)->args);
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
		write_here_doc(vars, vars->argv[2]);
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


	check_fd("main start");
	// int				save_stdin = dup(STDIN_FILENO);
	// int				save_stdout = dup(STDOUT_FILENO);
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
		(cmd + index)->redirection_in = -1;
		(cmd + index)->redirection_out = -1;
		// fprintf(stderr, "arg_len : %d\n", current_node->arg_len);
		(cmd + index)->args = malloc(sizeof(char *) * (current_node->arg_len + 1));
		parser_node = current_node->cmd_list_head;
		arg_index = 0;
		while (parser_node != NULL)
		{
			if (parser_node->type == REDIRECTION)
			{
				// 기본적인 오류 
				// ex) 리다이엑션 다음이 비어있을때 는 파싱부에서 처리
				// 바로 다음 오는 string이 filename 이라고 간주
				// open 에서 절대 / 상대 경로 다 열어줌
				if (parser_node->token[0] == '<')
				{
					parser_node = parser_node->next;
					if ((cmd + index)->redirection_in != -1)
						close((cmd + index)->redirection_in);
					read_file(&((cmd + index)->redirection_in),parser_node->token, O_RDONLY);
					fprintf(stderr, "< [%s]\n", parser_node->token);
				}
				else if (parser_node->token[0] == '>')
				{
					if ((cmd + index)->redirection_out != -1)
						close((cmd + index)->redirection_out);
					if (parser_node->token[1] == '>')
					{
						parser_node = parser_node->next;
						write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_APPEND);
						fprintf(stderr, "> [%s]\n", parser_node->token);
					}
					else
					{
						parser_node = parser_node->next;
						write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_TRUNC);
						fprintf(stderr, ">> [%s]\n", parser_node->token);
					}
				}
			}
			else if (parser_node->type == HERE_DOC)
			{
				// .here_doc 이라는 파일이름을 고정적으로 하지 않고 이미 파일이 존재한다면 다른 파일로 변경해야함
				parser_node = parser_node->next;
				write_file(&(vars->here_doc_fd), ".here_doc", \
					O_WRONLY | O_CREAT | O_TRUNC);
				fprintf(stderr, "<< [%s]\n", parser_node->token);
				write_here_doc(vars, parser_node->token);
				read_file(&((cmd + index)->redirection_in), ".here_doc", O_RDONLY);
			}
			else
			{
				(cmd + index)->args[arg_index] = parser_node->token;
				// fprintf(stderr, "arg %d : [%s]\n", arg_index, (cmd + index)->args[arg_index]);
				arg_index++;
			}
			parser_node = parser_node->next;
		}
		(cmd + index)->args[arg_index] = NULL;
		current_node = current_node->next;
		index++;
	}

	// 일단 argv 로 받다가 가장 앞의 인자를 name이라고 간주한뒤 빌트인 -> local -> path 로 찾아나감
	current_node = tree;
	index = 0;
	while (current_node != NULL)
	{
		parser_node = current_node->cmd_list_head;
		arg_index = 0;
		while (parser_node != NULL)
		{
			if (parser_node->type == STRING)
			{
				(cmd + index)->cmd_name = parser_node->token;
				break;
			}
			parser_node = parser_node->next;
		}
		current_node = current_node->next;
		index++;
	}

	check_fd("end cmd parsing");
	int		count;
	pid_t	fork_ret;

	// 리다이엑션이 들어오면 STDIN_FILENO 이 아니라 file1_read_fd로
	// 위의 t_parser_list -> cmd 로 바꾸는 과정에서 prev_read를 바꾸어줌

// if ((cmd + count)->redirection == 1)
	// 	vars->prev_read = open(file_path, OPTION)
	// else
		vars->prev_read = dup(STDIN_FILENO);
	count = 0;
	while (count < vars->cmd_len)
	{
		/*
		//cmd_init(vars, (cmd + count), count, \
			vars->argv[count + 2 + vars->is_here_doc]);
		*/
		(cmd + count)->envp = vars->envp;
		//parse_name_args(cmd, cmd_argv);
		fprintf(stderr, "cmd%d : %d %d\n",count, (cmd + count)->redirection_in, (cmd + count)->redirection_out);

		// 파이프는 마지막을 제외하고 열려있어야함
		if (count != vars->cmd_len - 1)
		{
			if (pipe(vars->pipe_fd) == -1)
				exit(EXIT_FAILURE);
		}
		// > 이 있으면 먼저 그쪽으로 넘기고
		if ((cmd + count)->redirection_out != -1)
		{
			vars->next_write = dup((cmd + count)->redirection_out);
			close((cmd + count)->redirection_out);
		}
		else
		{
			// 아닐 경우, 다음에 파이프가 존재한다면 그쪽으로
			if (count != vars->cmd_len - 1)
			{
				vars->next_write = dup(vars->pipe_fd[1]);
				close(vars->pipe_fd[1]);
			}
			// 마지막일 경우 stdout 으로 넘김
			else
			{
				vars->next_write = dup(STDOUT_FILENO);
				//vars->next_write = dup(vars->file2_write_fd);
			}
		}
		
		// in 은 부모와 자식 다르게 설정
		fork_ret = fork();
		if (fork_ret == -1)
			exit(EXIT_FAILURE);
		else if (fork_ret == 0)
		{
			if (count != vars->cmd_len - 1)
				close(vars->pipe_fd[0]);
			child(vars, (cmd + count));
		}
		else
		{
			check_fd("parant");
			(cmd + count)->pid = fork_ret;
			close(vars->prev_read);
			close(vars->next_write);
			if (cmd->redirection_in != -1)
				close(cmd->redirection_in);
			if (count != vars->cmd_len - 1)
			{
				vars->prev_read = dup(vars->pipe_fd[0]);
				close(vars->pipe_fd[0]);
			}
		}
		count++;
	}
	status->exit_status = main_return(vars, cmd);


	// dup2(save_stdin, STDIN_FILENO);
	// dup2(save_stdout, STDOUT_FILENO);
	// close(save_stdin);
	// close(save_stdout);
	return (status->exit_status);
	//return (0);
}