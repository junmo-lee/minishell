#include "../minishell.h"

static int	count_pipe(t_parser_list *parser_list)
{
	t_parser_list	*current_node;
	int				pipe_count;

	current_node = parser_list;
	pipe_count = 0;
	while (current_node != NULL)
	{
		if (current_node->type == PIPE)
			pipe_count ++;
		current_node = current_node->next;
	}
	return (pipe_count);
}

t_parser_list	*find_cmd_head(t_parser_list *parser_list, int pipe_index)
{
	t_parser_list	*current_node;
	int				pipe_count;

	current_node = parser_list;
	pipe_count = 0;
	while (current_node != NULL)
	{
		if (current_node->type == PIPE)
			pipe_count ++;
		if (current_node->type != PIPE && pipe_count == pipe_index)
			return (current_node);
		current_node = current_node->next;
	}
	return (current_node);
}

static void	divide_cmd_list(t_parsed_tree **parsed_tree)
{
	t_parser_list	*current_node;

	current_node = (*parsed_tree)->cmd_list_head;
	while (current_node != NULL)
	{
		if (current_node->next != NULL && current_node->next->type == PIPE)
		{
			destroy_parse_node(current_node->next);
			current_node->next = NULL;
		}
		current_node = current_node->next;
	}
}

t_parsed_tree	*make_parsed_tree(t_parser_list *parser_list)
{
	int				pipe_count;
	t_parsed_tree	*head;
	t_parsed_tree	*node;
	int				index;
	t_parser_list	*cmd_list_head;

	index = 0;
	pipe_count = count_pipe(parser_list);
	cmd_list_head = NULL;
	head = NULL;
	while (index < pipe_count + 1)
	{
		cmd_list_head = find_cmd_head(parser_list, index);
		node = create_parsed_tree_node(pipe_count + 1, cmd_list_head);
		append_parsed_tree_node(&head, node);
		index ++;
	}
	node = head;
	while (node != NULL)
	{
		divide_cmd_list(&node);
		node = node->next;
	}
	return (head);
}
