#include "parser.h"

void	check_redirection_syntax(t_info *info_s, t_parser_list *parser_list)
{
	t_parser_list	*current_node;

	current_node = parser_list;
	while (current_node != NULL)
	{
		if (current_node->type == REDIRECTION || current_node->type == HERE_DOC)
		{
			if (current_node->next == NULL || current_node->next->type != STRING)
				info_s->error = REDIRECTION_ERROR;
		}
		current_node = current_node->next;
	}
}

void	check_pipe_syntax(t_info *info_s, t_parser_list *parser_list)
{
	t_parser_list	*current_node;
	tag				prev_type;

	current_node = parser_list;
	prev_type = DEFAULT;
	while (current_node != NULL)
	{
		if (current_node->type == PIPE)
		{
			if (prev_type != STRING || current_node->next == NULL || current_node->next->type != STRING)
				info_s->error = PIPE_ERROR;
		}
		prev_type = current_node->type;
		current_node = current_node->next;
	}
}

int		count_pipe(t_parser_list *parser_list)
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

void	check_syntax(t_info *info_s, t_parser_list *parse_list)
{
	check_redirection_syntax(info_s, parse_list);
	check_pipe_syntax(info_s, parse_list);
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

void	divide_cmd_list(t_parsed_tree **parsed_tree)
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

static void	init_info_struct(t_info *info_s)
{
	info_s->token_count = 0;
	info_s->start_index = 0;
	info_s->end_index = 0;
	info_s->type_code = DEFAULT;
	info_s->connect_flag = 0;
	info_s->error = 0;	
}

t_parsed_tree	*parser(char *str)
{
	t_info			info_s;
	t_token_list	*token_list;
	t_parser_list	*parse_list;
	t_parsed_tree	*parsed_tree;

	parsed_tree = NULL;
	init_info_struct(&info_s);
	token_list = tokenize_string(&info_s, str);
	expand_env_vars_in_token_list(&token_list); // 히어독 뒤에 나오는 딜리미터의 달러는 환경변수 처리 안 함. 히어독 나오면 딜리미터 끝날 때까지 더블쿠오트를 실글로 테그 변경
	parse_list = combine_expanded_tokens(&token_list);
	check_syntax(&info_s, parse_list);
	if (info_s.error != NO_ERROR)
	{
		parsed_tree = (t_parsed_tree *)malloc(sizeof(t_parsed_tree));
		parsed_tree->cmd_len = 0;
		parsed_tree->cmd_list_head = NULL;
		parsed_tree->error = info_s.error;
		parsed_tree->next = NULL;
	}
	else
	{
		parsed_tree = make_parsed_tree(parse_list);
	}
	free(str);
	str = NULL;
	clear_token_list(&token_list);
	// clear_parse_list(&parse_list);
	return (parsed_tree);
}
