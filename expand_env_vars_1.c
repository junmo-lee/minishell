#include "parser.h"

static void	expand_env_vars_started_with_alpha(char *str, t_info *s_info, t_envp_list *envp_list)
{
	int		last_text_idx;
	char	*envp_value;
	char	*static_text;

	last_text_idx = s_info->index - 1;
	envp_value = get_envp_value(str, &(s_info->index), envp_list);
	static_text = slice_string(s_info->first_text_idx, last_text_idx, str);
	s_info->expanded_token = concatenate_strings(s_info->expanded_token, static_text);
	s_info->expanded_token = concatenate_strings(s_info->expanded_token, envp_value);
	s_info->first_text_idx = s_info->index;
}

static void	expand_env_vars_started_with_digit(char *str, t_info *s_info)
{
	int		last_index_of_text;
	char	*envp_value;
	char	*static_text;

	last_index_of_text = s_info->index - 1;
	s_info->index += 2;
	envp_value = ft_strdup("");
	static_text = slice_string(s_info->first_text_idx, last_index_of_text, str);
	s_info->expanded_token = concatenate_strings(s_info->expanded_token, static_text);
	s_info->expanded_token = concatenate_strings(s_info->expanded_token, envp_value);
	s_info->first_text_idx = s_info->index;
}

static void	concatenate_text_without_env_vars(char *str, t_info *s_info)
{
	int		last_index_of_text;
	char	*static_text;

	last_index_of_text = s_info->index - 1;
	static_text = slice_string(s_info->first_text_idx, last_index_of_text, str);
	s_info->expanded_token = concatenate_strings(s_info->expanded_token, static_text);
}

static void	expand_env_vars_with_question_mark(char *str, t_info *s_info, t_status *status)
{
	int		last_index_of_text;
	char	*envp_value;
	char	*static_text;

	last_index_of_text = s_info->index - 1;
	s_info->index += 2;
	envp_value = ft_itoa(status->exit_status);
	static_text = slice_string(s_info->first_text_idx, last_index_of_text, str);
	s_info->expanded_token = concatenate_strings(s_info->expanded_token, static_text);
	s_info->expanded_token = concatenate_strings(s_info->expanded_token, envp_value);
	s_info->first_text_idx = s_info->index;
}

char	*expand_env_vars(char *token, t_status *status, t_envp_list *envp_list, t_info *s_info)
{
	s_info->index = 0;
	s_info->str_len = ft_strlen(token);
	s_info->expanded_token = ft_strdup("");
	s_info->first_text_idx = s_info->index;
	while (s_info->index < s_info->str_len)
	{
		if (token[s_info->index] == '$' && (ft_isalpha(token[s_info->index + 1]) || token[s_info->index + 1] == '_'))
			expand_env_vars_started_with_alpha(token, s_info, envp_list);
		else if (token[s_info->index] == '$' && ft_isdigit(token[s_info->index + 1]))
			expand_env_vars_started_with_digit(token, s_info);
		else if (token[s_info->index] == '$' && token[s_info->index + 1] == '?')
			expand_env_vars_with_question_mark(token, s_info, status);
		s_info->index ++;
	}
	concatenate_text_without_env_vars(token, s_info);
	free(token);
	return (s_info->expanded_token);
}

