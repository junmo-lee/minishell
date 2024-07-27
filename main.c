#include "parser.h"

// void	leaks_check(void)
// {
// 	system("leaks parse");
// }

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		// clear_undo가 1로 설정된 경우:
		// 현재 입력 줄이 새로운 내용으로 교체되고, 이전의 편집 히스토리(undo 히스토리)는 삭제됩니다. 따라서 사용자는 더 이상 이전 상태로 되돌릴 수 없습니다.
		// clear_undo가 0으로 설정된 경우:
		// 현재 입력 줄이 새로운 내용으로 교체되지만, 이전의 편집 히스토리는 유지됩니다. 따라서 사용자는 여전히 undo 기능을 사용할 수 있습니다.
		rl_replace_line("", 0);
		rl_redisplay();
		// status->exit_status = SIGINT_EXIT_CODE;
		// 전역변수에서 시그널을 받아와야 할듯
	}
	if (signo == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		// status->exit_status = SIGQUIT_EXIT_CODE;
	}
}

void	printf_parsed_tree(t_parsed_tree *head)
{
	t_parsed_tree	*current_node;
	t_parser_list	*parser_node;
	int				index;

	current_node = head;
	index = 0;
	fprintf(stderr, "cmd_len : %d\n", current_node->cmd_len);
	while (current_node != NULL)
	{
		fprintf(stderr, "cmd_list %d\n", index);
		index ++;
		current_node->arg_len = 0;
		parser_node = current_node->cmd_list_head;
		while (parser_node != NULL)
		{
			if (parser_node->type == STRING)
			fprintf(stderr, "	%s		->STRING\n", parser_node->token);
			else if (parser_node->type == REDIRECTION)
			fprintf(stderr, "	%s		->REDIRECTION\n", parser_node->token);
			else if (parser_node->type == HERE_DOC)
			fprintf(stderr, "	%s		->HERE_DOC\n", parser_node->token);
			current_node->arg_len++;
			parser_node = parser_node->next;
		}
		current_node = current_node->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*str;
	struct termios	term;
	t_parsed_tree	*head;
	t_vars			vars;
	t_status		status;
	t_envp_list		*envp_list;
	
	ft_memset(&status, 0, sizeof(status));
	// main의 지역변수로 vars 관리
	status.one_line = &vars;

	//getcwd 로 현재 위치 pwd 에 저장
	if (getcwd(status.pwd, PATH_MAX) == NULL)
		exit(EXIT_FAILURE);
	// fprintf(stderr, "pwd : [%s]\n", status.pwd);

	envp_list = status.env_list;
	envp_list = get_envp(envp);
	// atexit(leaks_check);
	head = NULL;
	str = NULL;
	if (argc != 1)
	{
		//fprintf(stderr, "argument input error");
		return (0);
	}
	tcgetattr(STDIN_FILENO, &term); // 왜 두번 하지?
	term.c_lflag &= ~(ECHOCTL); // c_lflag는 input 관련 속성을 변경할 수 있다. ECHOCTL은 제어문자를 echo시킴 ~는 끄는 것일듯.
	tcsetattr(STDIN_FILENO, TCSANOW, &term); // 터미널 속성을 설정, TCSANOW는 "속성을 바로 병경한다"는 뜻
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	
	while (1)
	{
		str = readline("prompt : ");
		if (str == NULL)
		{
			ft_putstr_fd("\x1b[1A", STDOUT_FILENO);
			ft_putstr_fd("\033[9C", STDOUT_FILENO);
			write(STDOUT_FILENO, "exit\n", sizeof("exit\n"));
			break ;
		}
		if (*str != '\0')
		{
			add_history(str);
			head = parser(str, &status, envp_list);
			// printf_parsed_list(parsed_list);
			// clear_parse_list(&parsed_list);
			if (head->error == NO_ERROR)
			{
				printf_parsed_tree(head);
				// 명령어 실행
				// vars 에 argc, argv, envp, path를 넣는 단계
				init(status.one_line, argc, argv, envp);

				// 실제 line 실행부
				run_cmd_tree(&status, head);
				// cmd 가 하나로만 왔을때 main shell 에 영향을 미침
				if (vars.cmd_len == 1)
				{
					if (ft_strncmp(head->cmd_list_head->token, "export", 7) == 0)
						export(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "unset", 6) == 0)
						unset(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "env", 4) == 0)
						env(head->cmd_list_head, &envp_list);
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
			clear_parsed_tree(&head); // leaks 잡은거 merge할때 안 합쳐 진듯
			// free(str);
		}
	}
	(void)argc;
	(void)argv;
	(void)envp;
}
