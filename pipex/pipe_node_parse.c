/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_node_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:58:15 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	here_doc_node(t_vars *vars, t_cmd *cmd, t_parser_list **cur)
{
	if (cmd->redirection_in != -1)
		close(cmd->redirection_in);
	(*cur) = (*cur)->next;
	if (make_here_doc(vars, cmd, (*cur)->token) == SIGINT)
	{
		free(cmd->args);
		free(vars->temp_here_doc);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	redirection_in_node(t_cmd *cmd, t_parser_list **cur)
{
	(*cur) = (*cur)->next;
	if (cmd->redirection_in != -1)
		close(cmd->redirection_in);
	if (read_file(&(cmd->redirection_in), (*cur)->token, O_RDONLY))
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
		if (write_file(&(cmd->redirection_out), \
			(*cur)->token, O_WRONLY | O_CREAT | O_APPEND))
			cmd->redirection_fail = 1;
	}
	else
	{
		(*cur) = (*cur)->next;
		if (write_file(&(cmd->redirection_out), \
			(*cur)->token, O_WRONLY | O_CREAT | O_TRUNC))
			cmd->redirection_fail = 1;
	}
	return (EXIT_SUCCESS);
}

int	node_parse(t_vars *vars, t_cmd *cmd, t_parser_list **cur, int *arg_index)
{
	if ((*cur)->type == HERE_DOC)
	{
		if (here_doc_node(vars, cmd, cur) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if ((*cur)->type == REDIRECTION)
	{
		if (cmd->redirection_fail == 1)
			return (EXIT_BREAK);
		if ((*cur)->token[0] == '<')
			redirection_in_node(cmd, cur);
		else if ((*cur)->token[0] == '>')
			redirection_out_node(cmd, cur);
	}
	else
	{
		(cmd)->args[*arg_index] = (*cur)->token;
		(*arg_index)++;
	}
	return (EXIT_SUCCESS);
}
