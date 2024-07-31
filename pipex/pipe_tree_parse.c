#include "../parser.h"

int	list_parse(t_vars *vars, t_cmd *cmd, t_parsed_tree *current_node)
{
	t_parser_list	*parser_node;
	int				arg_index;
	int				node_parse_ret;

	parser_node = (current_node)->cmd_list_head;
	arg_index = 0;
	while (parser_node != NULL)
	{
		rl_done = 0;
		signal(SIGINT, stdin_handler);
		signal(SIGQUIT, stdin_handler);
		node_parse_ret = node_parse(vars, cmd, &parser_node, &arg_index);
		if (node_parse_ret == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (node_parse_ret == EXIT_BREAK)
			break ;
		parser_node = parser_node->next;
	}
	cmd->cmd_name = cmd->args[0];
	cmd->args[arg_index] = NULL;
	return (EXIT_SUCCESS);
}

int	tree_parse(t_parsed_tree *tree, t_vars *vars, t_cmd *cmd)
{
	int				index;
	t_parsed_tree	*current_node;

	current_node = tree;
	index = 0;
	while (current_node != NULL)
	{
		(cmd + index)->redirection_in = -1;
		(cmd + index)->redirection_out = -1;
		(cmd + index)->args = malloc(sizeof(char *) \
		* (current_node->arg_len + 1));
		if (list_parse(vars, cmd + index, current_node) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		current_node = current_node->next;
		index++;
	}
	return (EXIT_SUCCESS);
}
