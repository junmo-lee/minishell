#include "parser.h"

t_parsed_tree	*create_parsed_tree_node(int cmd_count, t_parser_list *cmd_list_head)
{
	t_parsed_tree	*new_node;

	new_node = (t_parsed_tree *)malloc(sizeof(t_parsed_tree));
	if (!new_node)
		return (NULL);
	new_node->cmd_len = cmd_count;
	new_node->cmd_list_head = cmd_list_head;
	new_node->error = NO_ERROR;
	new_node->next = NULL;
	return (new_node);
}

void	append_parsed_tree_node(t_parsed_tree **head, t_parsed_tree *node)
{
	t_parsed_tree	*tail;

	if ((*head) == NULL)
	{
		*head = node;
	}
	else
	{
		tail = (*head);
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		tail->next = node;
	}
}
