#include "../parser.h"

int	option_check(const char *str)
{
	int	index;

	index = 0;
	if (str[index] == '-')
		index ++;
	else
		return (0);
	while (str[index] != '\0')
	{
		if (str[index] != 'n')
			return (0);
		index ++;
	}
	return (1);
}

int	echo(t_parser_list *cmd_list)
{
	int				str_len;
	int				option_flag;
	t_parser_list	*current_node;

	str_len = 0;
	option_flag = 0;
	current_node = cmd_list->next;
	while (current_node != NULL)
	{
		if (option_check(current_node->token) == 1)
		{
			option_flag = 1;
			current_node = current_node->next;
			continue ;
		}
		str_len = strlen(current_node->token);
		write (STDOUT_FILENO, current_node->token, str_len); //에러 처리 나중에
		if (current_node->next == NULL && option_flag == 0)
			write (STDOUT_FILENO, "\n", 1);
		if (current_node->next != NULL)
			write (STDOUT_FILENO, " ", 1);
		current_node = current_node->next;
	}
	return (0);
}
