#include "parser.h"

static void	init_info_struct(t_info *info_s)
{
	info_s->token_count = 0;
	info_s->start_index = 0;
	info_s->end_index = 0;
	info_s->type_code = DEFAULT;
	info_s->connect_flag = 0;
	info_s->error = 0;
}

t_parser_list	*parser(char *str)
{
	t_info			info_s;
	t_token_list	*token_list;
	t_parser_list	*parse_list;

	init_info_struct(&info_s);
	token_list = tokenize_string(&info_s, str);
	expand_env_vars_in_token_list(&token_list);
	parse_list = combine_expanded_tokens(&token_list);
	free(str);
	clear_token_list(&token_list);
	return (parse_list);
}
