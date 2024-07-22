#include "parser.h"

t_token_list	*create_token_node(t_info *info_s, char *str)
{
	t_token_list	*new_node;

	new_node = (t_token_list *)malloc(sizeof(t_token_list));
	if (!new_node)
	{
		info_s->error = MALLOC_ERROR;
		return (NULL);
	}
	new_node->token = str;
	new_node->type = info_s->type_code;
	new_node->connect_flag = info_s->connect_flag;
	new_node->error = 0;
	new_node->next = NULL;
	return (new_node);
}

static void	destroy_token_node(t_token_list *node)
{
	free(node->token);
	node->token = NULL;
	free(node);
	node = NULL;
}

void	clear_token_list(t_token_list **head)
{
	t_token_list	*current_node;
	t_token_list	*next_node;

	current_node = *head;
	while (current_node != NULL)
	{
		next_node = current_node->next;
		destroy_token_node(current_node);
		current_node = next_node;
	}
}

void	append_token_node(t_token_list **head, t_token_list *node)
{
	t_token_list	*tail;

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
