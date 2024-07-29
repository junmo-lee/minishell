/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 18:53:05 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/29 22:13:10 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"
#include <signal.h>

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
				
				
				if ((cmd + count)->args != NULL)
					free((cmd + count)->args);
				
			}
			count++;
		}
	}
}

int	run_cmd_tree(t_status *status, t_parsed_tree *tree)
{
	t_cmd 			cmd[OPEN_MAX]; 
	t_parsed_tree	*current_node;
	t_parser_list	*parser_node;
	int				index;
	int				arg_index;
	t_vars			*vars;

	
	
	vars = status->one_line;
	vars->pwd = status->pwd;
	vars->cmd_len = tree->cmd_len;
	vars->is_here_doc = 0;
	
	
	
	
	
	

	ft_memset(cmd, 0, sizeof(cmd));
	current_node = tree;
	index = 0; 

	while (current_node != NULL)
	{
		(cmd + index)->redirection_in = -1;
		(cmd + index)->redirection_out = -1;
		
		(cmd + index)->args = malloc(sizeof(char *) * (current_node->arg_len + 1));
		parser_node = current_node->cmd_list_head;
		arg_index = 0;
		while (parser_node != NULL)
		{
			rl_done = 0;
			signal(SIGINT, stdin_handler);
			signal(SIGQUIT, stdin_handler);
			if (parser_node->type == HERE_DOC)
			{
				if ((cmd + index)->redirection_in != -1)
					close((cmd + index)->redirection_in);
				
				parser_node = parser_node->next;
				
				if (make_here_doc(vars, cmd + index, parser_node->token) == SIGINT)
				{
					return (EXIT_FAILURE);
				}
			}
			else if (parser_node->type == REDIRECTION)
			{
				if ((cmd + index)->redirection_fail == 1)
					break ;
				
				
				
				
				if (parser_node->token[0] == '<')
				{
					parser_node = parser_node->next;
					
					if ((cmd + index)->redirection_in != -1)
						close((cmd + index)->redirection_in);
					if (read_file(&((cmd + index)->redirection_in),parser_node->token, O_RDONLY))
						(cmd + index)->redirection_fail = 1;
					
				}
				else if (parser_node->token[0] == '>')
				{
					if ((cmd + index)->redirection_out != -1)
						close((cmd + index)->redirection_out);
					if (parser_node->token[1] == '>')
					{
						parser_node = parser_node->next;
						if (write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_APPEND))
							(cmd + index)->redirection_fail = 1;
						
					}
					else
					{
						parser_node = parser_node->next;
						if (write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_TRUNC))
							(cmd + index)->redirection_fail = 1;
						
					}
				}
			}
			else
			{
				(cmd + index)->args[arg_index] = parser_node->token;
				
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
		
			vars->argv[count + 2 + vars->is_here_doc]);
		*/
		(cmd + count)->envp = vars->envp;
		

		
		if (count != vars->cmd_len - 1)
		{
			if (pipe(vars->pipe_fd) == -1)
				exit(EXIT_FAILURE);
		}
		
		if ((cmd + count)->redirection_out != -1)
		{
			vars->next_write = dup((cmd + count)->redirection_out);
			close((cmd + count)->redirection_out);
		}
		else
		{
			
			if (count != vars->cmd_len - 1)
			{
				vars->next_write = dup(vars->pipe_fd[1]);
				close(vars->pipe_fd[1]);
			}
			
			else
			{
				vars->next_write = dup(STDOUT_FILENO);
			}
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
	free_strs(vars->path, EXIT_SUCCESS);
	status->exit_status = get_exit_status((cmd + (vars->cmd_len - 1))->status);
	
	return (status->exit_status);
	
}