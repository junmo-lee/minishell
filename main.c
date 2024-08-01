#include "parser.h"

void	leaks_check(void)
{
	system("leaks minishell");
}

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
	// fprintf(stderr, "cmd_len : %d\n", current_node->cmd_len);
	while (current_node != NULL)
	{
		// fprintf(stderr, "cmd_list %d\n", index);
		index ++;
		current_node->arg_len = 0;
		parser_node = current_node->cmd_list_head;
		while (parser_node != NULL)
		{
			// if (parser_node->type == STRING)
			// fprintf(stderr, "	%s		->STRING\n", parser_node->token);
			// else if (parser_node->type == REDIRECTION)
			// fprintf(stderr, "	%s		->REDIRECTION\n", parser_node->token);
			// else if (parser_node->type == HERE_DOC)
			// fprintf(stderr, "	%s		->HERE_DOC\n", parser_node->token);
			current_node->arg_len++;
			parser_node = parser_node->next;
		}
		current_node = current_node->next;
	}
}

void	main_status_init(t_status *status, t_vars *vars, t_envp_list **envp_list, char **envp)
{
	ft_memset(vars, 0, sizeof(vars));
	ft_memset(status, 0, sizeof(status));
	status->one_line = vars;
	status->env_list = get_envp(envp);
	*envp_list = status->env_list;

	if (getcwd(status->pwd, PATH_MAX) == NULL)
		exit(EXIT_FAILURE);
	insert_envp_node(envp_list, ft_strdup("OLDPWD"), NULL);
}



int	do_loop(t_status *status, t_vars *vars, t_envp_list *envp_list)
{
	char			*str;
	t_parsed_tree	*head;

	head = NULL;
	str = NULL;
	
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
			head = parser(str, status, envp_list);
			bubble_sort(envp_list);
			vars_init(status->one_line, envp_list);
			if (head->error == NO_ERROR)
			{
				printf_parsed_tree(head);
				g_signal = 0;
				status->exit_status = 0;
				status->env_list = envp_list;
				status->exit_status = run_cmd_tree(status, head, vars);
				if (vars->is_here_doc == 1)
				{
					unlink(vars->temp_here_doc);
					free(vars->temp_here_doc);
				}
				if (vars->cmd_len == 1)
				{
					if (ft_strncmp(head->cmd_list_head->token, "export", 7) == 0)
						export(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "unset", 6) == 0)
						unset(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "cd", 3) == 0)
						cd(head->cmd_list_head, &envp_list, status->pwd);
					else if (ft_strncmp(head->cmd_list_head->token, "exit", 5) == 0)
						builtin_exit(head->cmd_list_head->next, status);
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
			if (vars->path != NULL)
				free_strs(vars->path, EXIT_SUCCESS);
			if (vars->envp != NULL)
				free_strs(vars->envp, EXIT_SUCCESS);
			clear_parsed_tree(&head); // leaks 잡은거 merge할때 안 합쳐 진듯
		}
		free(str);
		str = NULL;
	}
	return (EXIT_SUCCESS);
}

int	prev_terminal_setting(t_status *status, t_vars *vars, t_envp_list *envp_list)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term); // 왜 두번 하지?
	term.c_lflag &= ~(ECHOCTL); // c_lflag는 input 관련 속성을 변경할 수 있다. ECHOCTL은 제어문자를 echo시킴 ~는 끄는 것일듯.
	tcsetattr(STDIN_FILENO, TCSANOW, &term); // 터미널 속성을 설정, TCSANOW는 "속성을 바로 병경한다"는 뜻
	return (do_loop(status, vars, envp_list));
}

int	main(int argc, char **argv, char **envp)
{
	atexit(leaks_check);
	t_status		status;
	t_vars			vars;
	t_envp_list		*envp_list;
	
	(void)argv;
	if (argc != 1)
	{
		//fprintf(stderr, "argument input error");
		return (0);
	}
	main_status_init(&status, &vars, &envp_list, envp);
	
	return (prev_terminal_setting(&status, &vars, envp_list));
}
