#include "../parser.h"

void	print_envp_in_export(t_envp_list *envp_list)
{
	while (envp_list != NULL)
	{
		write (1, envp_list->key, ft_strlen(envp_list->key));
		if (envp_list->value != NULL)
		{
			write (1, "=\"", 2);
			write (1, envp_list->value, ft_strlen(envp_list->value));
			write (1, "\"", 1);
		}
		write (1, "\n", 1);
		envp_list = envp_list->next;
	}
}

int	find_equal(char *str)
{
	while (*str != '\0')
	{
		if (*str == '=')
			return (1);
		str ++;
	}
	return (0);
}

int	check_key_syntax(char *key_str)
{
	int	index;

	if (key_str == NULL)
		return (UNDEFINED_ERROR);
	index = 0;
	if (ft_strlen(key_str) == 0)
		return (UNDEFINED_ERROR);
	if (ft_isdigit(key_str[index]) == 1)
		return (UNDEFINED_ERROR);
	while (key_str[index] != '\0')
	{
		if (ft_isalnum(key_str[index]) == 0 && key_str[index] != '_')
			return (UNDEFINED_ERROR);
		index ++;
	}
	return (0);
}

int	*export(t_parser_list *cmd_head, t_envp_list **envp_list)
{
	t_parser_list	*current_node;
	char			*key_str;
	char			*value_str;

	current_node = cmd_head->next;
	if (cmd_head->next == NULL)
	{
		// print_envp_in_export(*envp_list);
	}
	else
	{
		while (current_node != NULL)
		{
			key_str = get_key(current_node->token); // key 규칙 추가 
			if (check_key_syntax(key_str) == UNDEFINED_ERROR) // key규칙에 안 맞으면 에러메세지 띄어주고 다음 토큰으로 넘어 가는 듯
				{
					//printf("\'%s\' : not a valid identifier\n", current_node->token);
					free (key_str);
					current_node = current_node->next;
					continue ;
				}
			if (ft_strncmp(key_str, "_", 2) == 0)
			{
				free(key_str);
				current_node = current_node->next;
				continue ;
			}
			if (find_equal(current_node->token))
				value_str = get_value(current_node->token);
			else
				value_str = NULL;
			insert_envp_node(envp_list, key_str, value_str);
			current_node = current_node->next;
		}
	}
	return (0);
}
