#include "parser.h"

char	*slice_string(int start_index, int end_index, char *str)
{
	int		result_len;
	char	*result;

	result_len = end_index - start_index + 1;
	result = (char *)malloc(sizeof(char) * (result_len + 1));
	if (!result)
		return (NULL);
	strncpy(result, str + start_index, result_len);
	result[result_len] = '\0';
	return (result);
}

char	*get_envp_value(char *str, int *index)
{
	char	*envp_key;
	int		start_index_of_key;
	int		last_index_of_key;
	char	*envp_value;

	if (str[*index + 1] == '{')
		*index += 2;
	else
		*index += 1;
	start_index_of_key = *index;
	while (ft_isalnum(str[*index]) || str[*index] == '_')
		*index += 1;
	last_index_of_key = *index - 1;
	envp_key = slice_string(start_index_of_key, last_index_of_key, str);
	envp_value = getenv(envp_key);
	if (envp_value == NULL)
		envp_value = ft_strdup("");
	else
		envp_value = ft_strdup(envp_value);
	free(envp_key);
	envp_key = NULL;
	return (envp_value);
}

char	*concatenate_strings(char *word1, char *word2)
{
	int		word1_len;
	int		word2_len;
	char	*result;

	word1_len = ft_strlen(word1);
	word2_len = ft_strlen(word2);
	result = (char *)malloc(sizeof(char) * (word1_len + word2_len + 1));
	ft_strlcpy(result, word1, word1_len + 1);
	ft_strlcat(result, word2, word1_len + word2_len + 1);
	free(word1);
	free(word2);
	return (result);
}
