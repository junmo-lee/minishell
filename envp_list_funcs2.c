#include "parser.h"

int	insert_envp_node(t_envp_list **head, char *key_str, char *value_str)
{
	t_envp_list	*current_node;
	t_envp_list *new_node;

	current_node = *(head);
	while (current_node != NULL)
	{
		if (ft_strncmp(current_node->key, key_str, \
		ft_strlen(current_node->key) + 1) == 0)
		{
			if (!(value_str == NULL && current_node->value != NULL))
			{
				free(current_node->value);
				current_node->value = value_str;
			}
			free (key_str);
			return (1);
		}
		current_node = current_node->next;
	}
	new_node = create_envp_node(key_str, value_str);
	append_envp_node(head, new_node);
	return (0);
}

char	*get_key(char *str)
{
	char	*key_str;
	int		index_equal;

	index_equal = 0;
	while (str[index_equal] != '=' && str[index_equal] != '\0')
		index_equal ++;
	key_str = (char *)malloc(sizeof(char) * (index_equal + 1));
	strlcpy(key_str, str, index_equal + 1);
	return (key_str);
}

char	*get_value(char *str)
{
	char	*value_str;

	while (*str != '=' && *str != '\0')
		str ++;
	str ++;
	if (*str == '\0')
		value_str = ft_strdup("");
	else
		value_str = (char *)malloc(sizeof(char) * strlen(&(*str)) + 1);
	strlcpy(value_str, str, strlen(&(*str)) + 1);
	return (value_str);
}

char	*ft_getenv(char *key_str, t_envp_list *envp_list)
{
	while (envp_list != NULL)
	{
		if (ft_strncmp(key_str, envp_list->key, ft_strlen(key_str) + 1) == 0)
			return (envp_list->value);
		envp_list = envp_list->next;
	}
	return (NULL);
}

t_envp_list	*get_envp(char **envp)
{
	int			index;
	char		*key;
	char		*value;
	t_envp_list	*tem_node;
	t_envp_list	*head;

	index = 0;
	head = NULL;
	while (envp[index] != NULL)
	{
		key = get_key(envp[index]);
		value = get_value(envp[index]);
		tem_node = create_envp_node(key, value);
		append_envp_node(&head, tem_node);
		index ++;
	}
	return (head);
}
