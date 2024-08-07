/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:17:04 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/07 00:29:36 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*slice_string(int start_index, int end_index, char *str)
{
	int		result_len;
	char	*result;

	result_len = end_index - start_index + 1;
	result = (char *)malloc(sizeof(char) * (result_len + 1));
	if (!result)
		return (NULL);
	ft_strlcpy(result, str + start_index, result_len + 1);
	result[result_len] = '\0';
	return (result);
}

char	*get_envp_value(char *str, int *index, t_envp_list *envp_list)
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
	envp_value = ft_getenv(envp_key, envp_list);
	if (envp_value == NULL)
		envp_value = ft_strdup("");
	else
		envp_value = ft_strdup(envp_value);
	free(envp_key);
	envp_key = NULL;
	return (envp_value);
}

char	*concat_strs(char *word1, char *word2)
{
	int		word1_len;
	int		word2_len;
	char	*result;

	word1_len = ft_strlen(word1);
	word2_len = ft_strlen(word2);
	result = (char *)malloc(sizeof(char) * (word1_len + word2_len + 1));
	if (!result)
		return (NULL);
	ft_strlcpy(result, word1, word1_len + 1);
	ft_strlcat(result, word2, word1_len + word2_len + 1);
	free(word1);
	free(word2);
	return (result);
}

void	handle_dilimiter_with_env(t_token_list **token_list)
{
	int				flag;
	t_token_list	*current_node;

	flag = 0;
	current_node = *(token_list);
	while (current_node != NULL)
	{
		if (current_node->type == HERE_DOC)
			flag = 1;
		if (flag == 1 && (current_node->type == DOUBLEQUOTE \
		|| current_node->type == STRING))
			current_node->type = SINGLEQUOTE;
		if (current_node->type != HERE_DOC && current_node->connect_flag == 0)
			flag = 0;
		current_node = current_node->next;
	}
}

void	expand_env_vars_in_token_list(t_token_list **token_s, t_status *status, \
t_envp_list *envp_list, t_info *s_info)
{
	t_token_list	*head;
	t_token_list	*node;

	head = (*token_s);
	node = head;
	while (node != NULL)
	{
		if (node->type == STRING || node->type == DOUBLEQUOTE)
		{
			node->token = expand_env_vars(node->token, status, \
			envp_list, s_info);
		}
		node = node->next;
	}
}
