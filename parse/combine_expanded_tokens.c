#include "../minishell.h"

static void	concat_tokens_create_node(char **tem_word1, char *token, \
t_token_list *current_node, t_parser_list **parser_list_head)
{
	char			*tem_word2;
	char			*tem_str;
	t_parser_list	*tem_node;

	tem_word2 = ft_strdup(token);
	tem_str = concat_strs(*tem_word1, tem_word2);
	tem_node = create_parse_node(tem_str, current_node);
	*tem_word1 = ft_strdup("");
	append_parse_node(parser_list_head, tem_node);
}

static void	concat_tokens(char **tem_word1, char *token)
{
	char			*tem_word2;

	tem_word2 = ft_strdup(token);
	*tem_word1 = concat_strs(*tem_word1, tem_word2);
}

static void	create_node(char **tem_word1, t_token_list *current_node, \
t_parser_list **parser_list_head)
{
	t_parser_list	*tem_node;

	tem_node = create_parse_node(*tem_word1, current_node);
	append_parse_node(parser_list_head, tem_node);
}

t_parser_list	*combine_expanded_tokens(t_token_list **token_s)
{
	t_token_list	*current_node;
	int				prev_connect_flag;
	char			*tem_word1;
	t_parser_list	*parser_list_head;

	current_node = (*token_s);
	prev_connect_flag = 0;
	tem_word1 = ft_strdup("");
	parser_list_head = NULL;
	while (current_node != NULL)
	{
		prev_connect_flag = current_node->connect_flag;
		if (prev_connect_flag == 0)
			concat_tokens_create_node(&tem_word1, current_node->token, \
			current_node, &parser_list_head);
		else if (prev_connect_flag == 1)
			concat_tokens(&tem_word1, current_node->token);
		if (prev_connect_flag == 1 && current_node->next == NULL)
			create_node(&tem_word1, current_node, &parser_list_head);
		current_node = current_node->next;
	}
	if (prev_connect_flag == 0)
		free(tem_word1);
	return (parser_list_head);
}
