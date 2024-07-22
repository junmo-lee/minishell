#include "parser.h"

t_token_list	*clear_token_list_return_null(t_token_list **head)
{
	clear_token_list(head);
	return (NULL);
}

static void	find_first_index_of_token(t_info *info_s, char *str)
{
	int	index;

	index = info_s->start_index;
	while (str[index] != '\0' && str[index] == ' ')
	{
		index ++;
	}
	info_s->start_index = index;
}

static void	identify_meta_char(t_info *info_s, char *str)
{
	info_s->end_index = info_s->start_index;
	if (str[info_s->start_index] == '|')
		info_s->type_code = PIPE;
	else if (str[info_s->start_index] == '<' \
	&& str[info_s->start_index + 1] == '<') // == "<<"
	{
		info_s->type_code = HERE_DOC;
		info_s->end_index += 1;
	}
	else if (str[info_s->start_index] == '<' \
	|| str[info_s->start_index] == '>') 
	{
		if (str[info_s->start_index] == '>' \
		&& str[info_s->start_index + 1] == '>') // == ">>"
			info_s->end_index += 1;
		info_s->type_code = REDIRECTION;
	}
	else
		info_s->type_code = STRING;
}


static char *extract_token(t_info *info_s, char *str)
{
	int		token_len;
	char	*result;
	int		start_index;

	start_index = info_s->start_index;
	token_len = info_s->end_index - start_index + 1;
	if (info_s->type_code == SINGLEQUOTE || info_s->type_code == DOUBLEQUOTE)
	{
		token_len -= 2;
		start_index += 1;
	}
	result = (char *)malloc(sizeof(char) * (token_len + 1));
	if (!result)
	{
		info_s->error = MALLOC_ERROR;
		return (NULL);
	}
	ft_strlcpy(result, str + start_index, token_len + 1);
	return (result);
}

t_token_list	*tokenize_string(t_info *info_s, char *str)
{
	char			*token;
	int				str_len;
	t_token_list	*new_node;
	t_token_list	*head;

	head = NULL;
	str_len = ft_strlen(str);
	while (info_s->start_index < str_len)
	{
		info_s->connect_flag = 0;
		find_first_index_of_token(info_s, str);
		identify_meta_char(info_s, str);
		if (info_s->type_code == STRING)
			find_last_index_of_token(info_s, str);
		if (info_s->error != NO_ERROR)
			return (clear_token_list_return_null(&head));
		token = extract_token(info_s, str);
		new_node = create_token_node(info_s, token);
		if (info_s->error != NO_ERROR)
			return (clear_token_list_return_null(&head));
		append_token_node(&head, new_node);
		info_s->start_index = info_s->end_index + 1;
	}
	return (head);
}
