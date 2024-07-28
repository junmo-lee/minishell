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
			write (1, current_node->key, ft_strlen(current_node->key));
			write (1, "=", 1);
			write (1, current_node->value, ft_strlen(current_node->value));
			write (1, "\n", 1);
			current_node = current_node->next;
		}
	}
	return (0);
}
