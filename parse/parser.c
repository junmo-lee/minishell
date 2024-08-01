/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:17:04 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_info_struct(t_info *info_s)
{
	info_s->index = 0;
	info_s->str_len = 0;
	info_s->token_count = 0;
	info_s->start_index = 0;
	info_s->end_index = 0;
	info_s->type_code = DEFAULT;
	info_s->connect_flag = 0;
	info_s->first_text_idx = 0;
	info_s->expanded_token = NULL;
	info_s->error = 0;
}

t_parsed_tree	*parser(char *str, t_status *status, t_envp_list *envp_list)
{
	t_info			info_s;
	t_token_list	*token_list;
	t_parser_list	*parse_list;
	t_parsed_tree	*parsed_tree;

	parsed_tree = NULL;
	init_info_struct(&info_s);
	token_list = tokenize_string(&info_s, str);
	handle_dilimiter_with_env(&token_list);
	expand_env_vars_in_token_list(&token_list, status, envp_list, &info_s);
	parse_list = combine_expanded_tokens(&token_list);
	check_syntax(&info_s, parse_list);
	if (info_s.error != NO_ERROR)
	{
		parsed_tree = create_parsed_tree_node(0, NULL);
		parsed_tree->error = info_s.error;
		clear_parse_list(&parse_list);
	}
	else
		parsed_tree = make_parsed_tree(parse_list);
	if (parsed_tree->cmd_list_head == NULL && parsed_tree->error == NO_ERROR)
		parsed_tree->cmd_len = 0;
	clear_token_list(&token_list);
	return (parsed_tree);
}
