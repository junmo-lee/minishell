#include "../parser.h"

int	here_doc_node(t_vars *vars, t_cmd *cmd, t_parser_list **cur)
{
	if (cmd->redirection_in != -1)
		close(cmd->redirection_in);
	(*cur) = (*cur)->next;
	if (make_here_doc(vars, cmd, (*cur)->token) == SIGINT)
	{
		free(cmd->args);
		free(vars->temp_here_doc);
		return (EXIT_FAILURE); // run_cmd_tree가 끝나야함
	}
	return (EXIT_SUCCESS);
}

int	redirection_in_node(t_cmd *cmd, t_parser_list **cur)
{
	(*cur) = (*cur)->next;
	if (cmd->redirection_in != -1)
		close(cmd->redirection_in);
	if (read_file(&(cmd->redirection_in),(*cur)->token, O_RDONLY))
		cmd->redirection_fail = 1;
	return (EXIT_SUCCESS);
}

int	redirection_out_node(t_cmd *cmd, t_parser_list **cur)
{
	if (cmd->redirection_out != -1)
		close(cmd->redirection_out);
	if ((*cur)->token[1] == '>')
	{
		(*cur) = (*cur)->next;
		if (write_file(&(cmd->redirection_out),(*cur)->token, O_WRONLY | O_CREAT | O_APPEND))
			cmd->redirection_fail = 1;
	}
	else
	{
		(*cur) = (*cur)->next;
		if (write_file(&(cmd->redirection_out),(*cur)->token, O_WRONLY | O_CREAT | O_TRUNC))
			cmd->redirection_fail = 1;
	}
	return (EXIT_SUCCESS);
}

int node_parse(t_vars *vars, t_cmd *cmd, t_parser_list	**cur, int *arg_index)
{
	// t_parser_list	*parser_node;

	// parser_node = *cur;
	if ((*cur)->type == HERE_DOC)
	{
		if (here_doc_node(vars, cmd, cur) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		// if (cmd->redirection_in != -1)
		// 	close(cmd->redirection_in);
		// (*cur) = (*cur)->next;
		// if (make_here_doc(vars, cmd, (*cur)->token) == SIGINT)
		// {
		// 	free(cmd->args);
		// 	free(vars->temp_here_doc);
		// 	return (EXIT_FAILURE); // run_cmd_tree가 끝나야함
		// }
	}
	else if ((*cur)->type == REDIRECTION)
	{
		if (cmd->redirection_fail == 1)
			return (EXIT_BREAK); //list_parse 가 끝나야함
		if ((*cur)->token[0] == '<')
		{
			redirection_in_node(cmd, cur);
			// (*cur) = (*cur)->next;
			// if (cmd->redirection_in != -1)
			// 	close(cmd->redirection_in);
			// if (read_file(&(cmd->redirection_in),(*cur)->token, O_RDONLY))
			// 	cmd->redirection_fail = 1;
		}
		else if ((*cur)->token[0] == '>')
		{
			redirection_out_node(cmd, cur);
			// if (cmd->redirection_out != -1)
			// 	close(cmd->redirection_out);
			// if ((*cur)->token[1] == '>')
			// {
			// 	(*cur) = (*cur)->next;
			// 	if (write_file(&(cmd->redirection_out),(*cur)->token, O_WRONLY | O_CREAT | O_APPEND))
			// 		cmd->redirection_fail = 1;
			// }
			// else
			// {
			// 	(*cur) = (*cur)->next;
			// 	if (write_file(&(cmd->redirection_out),(*cur)->token, O_WRONLY | O_CREAT | O_TRUNC))
			// 		cmd->redirection_fail = 1;
			// }
		}
	}
	else
	{
		(cmd)->args[*arg_index] = (*cur)->token;
		(*arg_index)++;
	}
	return (EXIT_SUCCESS);
}