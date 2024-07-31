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
		// if (parser_node->type == HERE_DOC)
		// {
		// 	if (cmd->redirection_in != -1)
		// 		close(cmd->redirection_in);
		// 	parser_node = parser_node->next;
		// 	if (make_here_doc(vars, cmd, parser_node->token) == SIGINT)
		// 	{
		// 		free(cmd->args);
		// 		free(vars->temp_here_doc);
		// 		return (EXIT_FAILURE); // run_cmd_tree가 끝나야함
		// 	}
		// }
		// else if (parser_node->type == REDIRECTION)
		// {
		// 	if (cmd->redirection_fail == 1)
		// 		return (EXIT_SUCCESS); //list_parse 가 끝나야함
		// 	if (parser_node->token[0] == '<')
		// 	{
		// 		parser_node = parser_node->next;
		// 		if (cmd->redirection_in != -1)
		// 			close(cmd->redirection_in);
		// 		if (read_file(&(cmd->redirection_in),parser_node->token, O_RDONLY))
		// 			cmd->redirection_fail = 1;
		// 	}
		// 	else if (parser_node->token[0] == '>')
		// 	{
		// 		if (cmd->redirection_out != -1)
		// 			close(cmd->redirection_out);
		// 		if (parser_node->token[1] == '>')
		// 		{
		// 			parser_node = parser_node->next;
		// 			if (write_file(&(cmd->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_APPEND))
		// 				cmd->redirection_fail = 1;
		// 		}
		// 		else
		// 		{
		// 			parser_node = parser_node->next;
		// 			if (write_file(&(cmd->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_TRUNC))
		// 				cmd->redirection_fail = 1;
		// 		}
		// 	}
		// }
		// else
		// {
		// 	(cmd)->args[arg_index] = parser_node->token;
		// 	arg_index++;
		// }
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
		(cmd + index)->args = malloc(sizeof(char *) * (current_node->arg_len + 1));
		// parser_node = current_node->cmd_list_head;
		// arg_index = 0;
		// while (parser_node != NULL)
		// {
		// 	rl_done = 0;
		// 	signal(SIGINT, stdin_handler);
		// 	signal(SIGQUIT, stdin_handler);
		// 	if (parser_node->type == HERE_DOC)
		// 	{
		// 		if ((cmd + index)->redirection_in != -1)
		// 			close((cmd + index)->redirection_in);
		// 		parser_node = parser_node->next;
		// 		if (make_here_doc(vars, cmd + index, parser_node->token) == SIGINT)
		// 		{
		// 			free((cmd + index)->args);
		// 			free(vars->temp_here_doc);
		// 			return (EXIT_FAILURE);
		// 		}
		// 	}
		// 	else if (parser_node->type == REDIRECTION)
		// 	{
		// 		if ((cmd + index)->redirection_fail == 1)
		// 			break ;
		// 		if (parser_node->token[0] == '<')
		// 		{
		// 			parser_node = parser_node->next;
		// 			if ((cmd + index)->redirection_in != -1)
		// 				close((cmd + index)->redirection_in);
		// 			if (read_file(&((cmd + index)->redirection_in),parser_node->token, O_RDONLY))
		// 				(cmd + index)->redirection_fail = 1;
		// 		}
		// 		else if (parser_node->token[0] == '>')
		// 		{
		// 			if ((cmd + index)->redirection_out != -1)
		// 				close((cmd + index)->redirection_out);
		// 			if (parser_node->token[1] == '>')
		// 			{
		// 				parser_node = parser_node->next;
		// 				if (write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_APPEND))
		// 					(cmd + index)->redirection_fail = 1;
		// 			}
		// 			else
		// 			{
		// 				parser_node = parser_node->next;
		// 				if (write_file(&((cmd + index)->redirection_out),parser_node->token, O_WRONLY | O_CREAT | O_TRUNC))
		// 					(cmd + index)->redirection_fail = 1;
		// 			}
		// 		}
		// 	}
		// 	else
		// 	{
		// 		(cmd + index)->args[arg_index] = parser_node->token;
		// 		arg_index++;
		// 	}
		// 	parser_node = parser_node->next;
		// }
		// (cmd + index)->cmd_name = (cmd + index)->args[0];
		// (cmd + index)->args[arg_index] = NULL;
		if (list_parse(vars, cmd + index, current_node) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		
		current_node = current_node->next;
		index++;
	}
	return (EXIT_SUCCESS);
}