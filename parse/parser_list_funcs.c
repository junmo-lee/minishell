#include "../minishell.h"

t_parser_list	*create_parse_node(char *str, t_token_list *token_s)
{
	t_parser_list	*new_node;

	new_node = (t_parser_list *)malloc(sizeof(t_parser_list));
	if (!new_node)
		return (NULL);
	if (token_s == NULL)
	{
		new_node->token = NULL;
		new_node->type = DEFAULT;
		new_node->error = UNDEFINED_ERROR;
		new_node->next = NULL;
	}
	else
	{
		new_node->token = str;
		if (token_s->type == SINGLEQUOTE || token_s->type == DOUBLEQUOTE)
			new_node->type = STRING;
		else
			new_node->type = token_s->type;
		new_node->error = token_s->error;
		new_node->next = NULL;
	}
	return (new_node);
}

void	destroy_parse_node(t_parser_list *node)
{
	free(node->token);
	node->token = NULL;
	free(node);
	node = NULL;
}

void	clear_parse_list(t_parser_list **head)
{
	t_parser_list	*current_node;
	t_parser_list	*next_node;

	if (head == NULL || *(head) == NULL)
		return ;
	current_node = *head;
	while (current_node != NULL)
	{
		next_node = current_node->next;
		destroy_parse_node(current_node);
		current_node = next_node;
	}
}

void	append_parse_node(t_parser_list **head, t_parser_list *node)
{
	t_parser_list	*tail;

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
