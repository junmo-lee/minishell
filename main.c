#include "parser.h"

// void	leaks_check(void)
// {
// 	system("leaks parse");
// }

void	printf_parsed_list(t_parser_list *head)
{
	t_parser_list	*current_node;

	current_node = head;
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

int	main(int argc, char **argv, char **envp)
{
	t_parser_list	*parsed_list;
	char	*str;

	// atexit(leaks_check);
	parsed_list = NULL;
	str = NULL;
	str = readline("prompt : ");
	parsed_list = parser(str);
	printf_parsed_list(parsed_list);
	// clear_token_list(&parsed_list);
	clear_parse_s(&parsed_list);
	(void)argc;
	(void)argv;
	(void)envp;
}
