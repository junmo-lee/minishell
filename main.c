#include "parser.h"

// void	leaks_check(void)
// {
// 	system("leaks parse");
// }

void	printf_parsed_list(t_parser_list *head)
{
	t_parser_list	*current_node;

	current_node = head;
	if (current_node->error != NO_ERROR)
		printf("error : %d\n", current_node->error);
	else
	{
		while (current_node != NULL)
		{
			if (current_node->type == STRING)
				printf("%s				-> string\n", current_node->token);
			else if (current_node->type == EQUAL)
				printf("%s				-> equal\n", current_node->token);
			else if (current_node->type == REDIRECTION)
				printf("%s				-> rediection\n", current_node->token);
			else if (current_node->type == HERE_DOC)
				printf("%s				-> here_doc\n", current_node->token);
			else if (current_node->type == PIPE)
				printf("%s				-> pipe\n", current_node->token);
			else if (current_node->type == SINGLEQUOTE)
				printf("%s				-> singlequote\n", current_node->token);
			else if (current_node->type == DOUBLEQUOTE)
				printf("%s				-> doublequote\n", current_node->token);
			current_node = current_node->next;
		}	
	}
}

void	printf_parsed_tree(t_parsed_tree *head)
{
	t_parsed_tree	*current_node;
	t_parser_list	*parser_node;
	int				index;

	current_node = head;
	index = 0;
	printf("cmd_len : %d\n", current_node->cmd_len);
	while (current_node != NULL)
	{
		printf("cmd_list %d\n", index);
		index ++;
		parser_node = current_node->cmd_list_head;
		while (parser_node != NULL)
		{
			if (parser_node->type == STRING)
				printf("	%s		->STRING\n", parser_node->token);
			else if (parser_node->type == REDIRECTION)
				printf("	%s		->REDIRECTION\n", parser_node->token);
			else if (parser_node->type == HERE_DOC)
				printf("	%s		->HERE_DOC\n", parser_node->token);
			parser_node = parser_node->next;
		}
		current_node = current_node->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*str;
	// struct termios	term;
	t_parsed_tree	*head;
	t_vars			vars;

	// atexit(leaks_check);
	head = NULL;
	str = NULL;
	if (argc != 1)
	{
		printf("argument input error");
		return (0);
	}
	// tcgetattr(STDIN_FILENO, &term); // 왜 두번 하지?
	// term.c_lflag &= ~(ECHOCTL); // c_lflag는 input 관련 속성을 변경할 수 있다. ECHOCTL은 제어문자를 echo시킴 ~는 끄는 것일듯.
	// tcsetattr(STDIN_FILENO, TCSANOW, &term); // 터미널 속성을 설정, TCSANOW는 "속성을 바로 병경한다"는 뜻
	while (1)
	{
		str = readline("prompt : ");
		if (!str)
			return (0);
		if (*str != '\0')
		{
			add_history(str);
			head = parser(str);
			// printf_parsed_list(parsed_list);
			// clear_parse_list(&parsed_list);
			if (head->error == NO_ERROR)
			{
				printf_parsed_tree(head);
				// 명령어 실행
				// vars 에 argc, argv, envp, path를 넣는 단계
				init(&vars, argc, argv, envp);

				// 실제 line 실행부
				
				// 명령어가 종료될때까지 기다림
			}
			else
			{
				if (head->error == NOT_CLOSED_ERROR)
					printf("NOT_CLOSED_ERROR\n");
				else if (head->error == MALLOC_ERROR)
					printf("MALLOC_ERROR\n");
				else if (head->error == REDIRECTION_ERROR)
					printf("REDIRECTION_ERROR\n");
				else if (head->error == PIPE_ERROR)
					printf("PIPE_ERROR\n");
			}
			// free(str);
		}
	}
	(void)argc;
	(void)argv;
	(void)envp;
}
