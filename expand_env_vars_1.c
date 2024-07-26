#include "parser.h"

static void	expand_env_vars_started_with_alpha(char *str, int *index, \
int *first_text_idx, char **expanded_token, t_envp_list *envp_list)
{
	int		last_text_idx;
	char	*envp_value;
	char	*static_text;

	last_text_idx = *index - 1;
	envp_value = get_envp_value(str, index, envp_list);
	static_text = slice_string(*first_text_idx, last_text_idx, str);
	*expanded_token = concatenate_strings(*expanded_token, static_text);
	*expanded_token = concatenate_strings(*expanded_token, envp_value);
	*first_text_idx = *index;
}

static void	expand_env_vars_started_with_digit(char *str, int *index, \
int *first_text_idx, char **expanded_token)
{
	int		last_index_of_text;
	char	*envp_value;
	char	*static_text;

	last_index_of_text = *index - 1;
	*index += 2;
	envp_value = ft_strdup("");
	static_text = slice_string(*first_text_idx, last_index_of_text, str);
	*expanded_token = concatenate_strings(*expanded_token, static_text);
	*expanded_token = concatenate_strings(*expanded_token, envp_value);
	*first_text_idx = *index;
}

static void	concatenate_text_without_env_vars(char *str, int *index, \
int *first_text_idx, char **expanded_token)
{
	int		last_index_of_text;
	char	*static_text;

	last_index_of_text = *index - 1;
	static_text = slice_string(*first_text_idx, last_index_of_text, str);
	*expanded_token = concatenate_strings(*expanded_token, static_text);
}

static void	expand_env_vars_with_question_mark(char *str, int *index, int *first_text_idx, char **expanded_token, t_status *status)
{
	int		last_index_of_text;
	char	*envp_value;
	char	*static_text;

	last_index_of_text = *index - 1;
	*index += 2;
	envp_value = ft_itoa(status->exit_status);
	static_text = slice_string(*first_text_idx, last_index_of_text, str);
	*expanded_token = concatenate_strings(*expanded_token, static_text);
	*expanded_token = concatenate_strings(*expanded_token, envp_value);
	*first_text_idx = *index;
}

static char	*expand_env_vars(char *str, t_status *status, t_envp_list *envp_list)
{
	int		index;
	int		first_text_idx;
	int		str_len;
	char	*expanded_token;

	index = 0;
	str_len = ft_strlen(str);
	expanded_token = ft_strdup("");
	first_text_idx = index;
	while (index < str_len)
	{
		if (str[index] == '$' && (ft_isalpha(str[index + 1]) \
		|| str[index + 1] == '_'))
			expand_env_vars_started_with_alpha(str, &index, \
			&first_text_idx, &expanded_token, envp_list);
		else if (str[index] == '$' && ft_isdigit(str[index + 1]))
			expand_env_vars_started_with_digit(str, &index, \
			&first_text_idx, &expanded_token);
		else if (str[index] == '$' && str[index + 1] == '?')
			expand_env_vars_with_question_mark(str, &index, &first_text_idx, &expanded_token, status);
		index ++;
	}
	concatenate_text_without_env_vars(str, &index, \
	&first_text_idx, &expanded_token);
	free(str);
	return (expanded_token);
}

void	expand_env_vars_in_token_list(t_token_list **token_s, t_status *status, t_envp_list *envp_list)
{
	t_token_list	*head;
	t_token_list	*node;

	head = (*token_s);
	node = head;
	while (node != NULL)
	{
		if (node->type == STRING || node->type == DOUBLEQUOTE)
		{
			node->token = expand_env_vars(node->token, status, envp_list);
		}
		node = node->next;
	}
}
