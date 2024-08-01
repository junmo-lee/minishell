#include "../minishell.h"

static void	check_redirection_syntax(t_info *info_s, t_parser_list *parser_list)
{
	t_parser_list	*current_node;

	current_node = parser_list;
	while (current_node != NULL)
	{
		if (current_node->type == REDIRECTION || current_node->type == HERE_DOC)
		{
			if (current_node->next == NULL \
			|| current_node->next->type != STRING)
				info_s->error = REDIRECTION_ERROR;
		}
		current_node = current_node->next;
	}
}

static void	check_pipe_syntax(t_info *info_s, t_parser_list *parser_list)
{
	t_parser_list	*current_node;
	t_tag			prev_type;

	current_node = parser_list;
	prev_type = DEFAULT;
	while (current_node != NULL)
	{
		if (current_node->type == PIPE)
		{
			if (prev_type != STRING || current_node->next == NULL \
			|| !(current_node->next->type == STRING \
			|| current_node->next->type == HERE_DOC \
			|| current_node->next->type == REDIRECTION))
				info_s->error = PIPE_ERROR;
		}
		prev_type = current_node->type;
		current_node = current_node->next;
	}
}

void	check_syntax(t_info *info_s, t_parser_list *parse_list)
{
	check_redirection_syntax(info_s, parse_list);
	check_pipe_syntax(info_s, parse_list);
}
