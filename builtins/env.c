#include "../parser.h"

int	env(t_parser_list *cmd_list, t_envp_list **envp_list)
{
	t_envp_list *current_node;

	current_node = *(envp_list);
	if (cmd_list->next != NULL)
	{
		printf ("env: %s: No such file or directory\n", cmd_list->next->token);
		return (1);
	}
	else
	{
		while (current_node != NULL)
		{
			if (current_node->value == NULL)
			{
				current_node = current_node->next;
				continue ;
			}
			
			
			
			
			current_node = current_node->next;
		}
	}
	return (0);
}
