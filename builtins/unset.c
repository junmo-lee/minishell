#include "../minishell.h"

int	unset(t_parser_list *cmd_head, t_envp_list **envp_list)
{
	t_parser_list	*current_node;

	if (cmd_head == NULL || envp_list == NULL || *envp_list == NULL)
		return (0);
	current_node = cmd_head->next;
	if (current_node == NULL)
		return (0);
	while (current_node != NULL)
	{
		if (check_key_syntax(current_node->token) == UNDEFINED_ERROR)
		{
			current_node = current_node->next;
			continue ;
		}
		remove_node_by_key(envp_list, current_node->token);
		current_node = current_node->next;
	}
	return (0);
}
