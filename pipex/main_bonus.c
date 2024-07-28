/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choolee <choolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 18:53:05 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/28 18:40:44 by choolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"
#include <signal.h>
// 디버그용
#include <stdio.h>

void	stdin_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "^C\n", 4);
	}
	if (signo == SIGQUIT)
	{
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

int	run_cmd_tree(t_status *status, t_parsed_tree *tree)
{
	t_cmd 			cmd[1024]; // 나중에 연결리스트 형태로 변경?
	t_parsed_tree	*current_node;
	t_parser_list	*parser_node;
	int				index;
	int				arg_index;
	t_vars			*vars;

	// 메인에서의 지역변수를 가져와 쓰는 형태
	// vars : main에서 argc, argv, envp, path 를 받아옴
	vars = status->one_line;
	vars->pwd = status->pwd;
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
		// //fprintf(stderr, "arg_len : %d\n", current_node->arg_len);
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
						fprintf(stderr, ">> [%s]\n", parser_node->token);
					}
					else
					{
						parser_node = parser_node->next;
						write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_TRUNC);
						fprintf(stderr, "> [%s]\n", parser_node->token);
					}
				}
			}
			else if (parser_node->type == HERE_DOC)
			{
				// 이전에 here_doc 이나 리다이엑션을 받았는지 먼저 확인이 필요할거 같음
				parser_node = parser_node->next;
				switch (make_here_doc(vars, cmd + index, parser_node->token))
				{
				case SIGINT:
					close_all_fd(vars, cmd);
					free(vars->temp_here_doc);
					return (SIGINT_EXIT_CODE);
					break;
				case SIGQUIT:
					close_all_fd(vars, cmd);
					free(vars->temp_here_doc);
					return (SIGQUIT_EXIT_CODE);
					break;
				default:
					break;
				}
			}
			else
			{
				(cmd + index)->args[arg_index] = parser_node->token;
				// fprintf(stderr, "arg %d : [%s]\n", arg_index, (cmd + index)->args[arg_index]);
				arg_index++;
			}
			parser_node = parser_node->next;
		}
		(cmd + index)->cmd_name = (cmd + index)->args[0];
		(cmd + index)->args[arg_index] = NULL;
		current_node = current_node->next;
		index++;
	}

	int		count;
	pid_t	fork_ret;

	signal(SIGINT, stdin_handler);
	signal(SIGQUIT, stdin_handler);

	vars->prev_read = dup(STDIN_FILENO);
	count = 0;
	while (count < vars->cmd_len)
	{
		/*
		//cmd_init(vars, (cmd + count), count, \
			vars->argv[count + 2 + vars->is_here_doc]);
		*/
		(cmd + count)->envp = vars->envp;
		// //fprintf(stderr, "cmd%d : %d %d\n",count, (cmd + count)->redirection_in, (cmd + count)->redirection_out);

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
			{
				vars->prev_read = dup(vars->pipe_fd[0]);
				close(vars->pipe_fd[0]);
			}
		}
		count++;
	}

	wait_processes(vars, cmd);
	if (vars->is_here_doc == 1)
		unlink(vars->temp_here_doc);
	// free(vars->temp_here_doc);
	free_strs(vars->path, EXIT_SUCCESS);
	status->exit_status = get_exit_status((cmd + (vars->cmd_len - 1))->status);
	// check_fd("main");

	return (status->exit_status);
	//return (0);
}