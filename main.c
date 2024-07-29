#include "parser.h"






void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signo == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		
	}
}

void	printf_parsed_tree(t_parsed_tree *head)
{
	t_parsed_tree	*current_node;
	t_parser_list	*parser_node;
	int				index;

	current_node = head;
	index = 0;
	
	while (current_node != NULL)
	{
		
		index ++;
		current_node->arg_len = 0;
		parser_node = current_node->cmd_list_head;
		while (parser_node != NULL)
		{
			
			
			
			
			
			
			current_node->arg_len++;
			parser_node = parser_node->next;
		}
		current_node = current_node->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*str;
	t_parsed_tree	*head;
	t_vars			vars;
	t_status		status;
	t_envp_list		*envp_list;
	int				exit_val;
	
	ft_memset(&status, 0, sizeof(status));
	
	status.one_line = &vars;


	status.env_list = get_envp(envp);;
	envp_list = status.env_list;
	

	
	if (getcwd(status.pwd, PATH_MAX) == NULL)
		exit(EXIT_FAILURE);
	insert_envp_node(&envp_list, ft_strdup("OLDPWD"), NULL);
	
	head = NULL;
	str = NULL;
	if (argc != 1)
	{
		
		return (0);
	}
	
	struct termios	term;
	tcgetattr(STDIN_FILENO, &term); 
	term.c_lflag &= ~(ECHOCTL); 
	tcsetattr(STDIN_FILENO, TCSANOW, &term); 
	
	
	while (1)
	{
		rl_catch_signals = 0;
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
		
		str = readline("prompt : \001\033[s\002");
		
		if (str == NULL)
		{
			write(STDOUT_FILENO, "\033[u\033[1B\033[1A", 12);
			write(STDOUT_FILENO, "exit\n", sizeof("exit\n"));
			
			break ;
		}
		if (*str != '\0')
		{
			add_history(str);
			head = parser(str, &status, envp_list);
			
			
			if (head->error == NO_ERROR)
			{
				printf_parsed_tree(head);
				
				
				init(status.one_line, argc, argv, envp);

				
				status.exit_status = run_cmd_tree(&status, head);
				if (vars.is_here_doc == 1)
				{
					unlink(vars.temp_here_doc);
					free(vars.temp_here_doc);
				}
				
				if (vars.cmd_len == 1)
				{
					if (ft_strncmp(head->cmd_list_head->token, "export", 7) == 0)
						export(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "unset", 6) == 0)
						unset(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "env", 4) == 0)
						env(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "cd", 3) == 0)
						cd(head->cmd_list_head, &envp_list, status.pwd);
					else if (ft_strncmp(head->cmd_list_head->token, "exit", 5) == 0)
					{
						if (head->cmd_list_head->next == NULL)
							exit_val = 0;
						else if (atoi_check_num(head->cmd_list_head->next->token, &exit_val))
							exit_val = !(!(head->cmd_list_head->next->next));
						else
							exit_val = 255;
						clear_parsed_tree(&head);
						exit(exit_val);
					}


				}
				
				
			}
			else
			{
				if (head->error == NOT_CLOSED_ERROR)
				fprintf(stderr, "NOT_CLOSED_ERROR\n");
				else if (head->error == MALLOC_ERROR)
				fprintf(stderr, "MALLOC_ERROR\n");
				else if (head->error == REDIRECTION_ERROR)
				fprintf(stderr, "REDIRECTION_ERROR\n");
				else if (head->error == PIPE_ERROR)
				fprintf(stderr, "PIPE_ERROR\n");
			}
			clear_parsed_tree(&head); 
			
		}
	}
	(void)argc;
	(void)argv;
	(void)envp;
}
