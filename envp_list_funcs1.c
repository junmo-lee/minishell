#include "parser.h"

t_envp_list	*create_envp_node(char *key_str, char *value_str)
{
	t_envp_list	*new_node;

	new_node = (t_envp_list *)malloc(sizeof(t_envp_list));
	if (!new_node)
		return (NULL);
	new_node->key = key_str;
	new_node->value = value_str;
	new_node->next = NULL;
	return (new_node);
}

static void	destroy_envp_list_node(t_envp_list **node)
{
	t_envp_list	*current_node;

	if (node == NULL)
		return ;
	current_node = *(node);
	ft_memset(current_node->key, 0, ft_strlen(current_node->key));
	free(current_node->key);
	if (current_node->value != NULL)
	{
		ft_memset(current_node->value, 0, ft_strlen(current_node->value));
		free(current_node->value);
	}
	free(current_node);
	current_node = NULL;
}

void	clear_envp_list(t_envp_list **head)
{
	t_envp_list	*current_node;
	t_envp_list	*next_node;

	if (head == NULL)
		return ;
	current_node = *(head);
	while (current_node != NULL)
	{
		next_node = current_node->next;
		destroy_envp_list_node(&current_node);
		current_node = next_node;
	}
}

void	remove_node_by_key(t_envp_list **head, char *key_str)
{
	t_envp_list	*current_node;
	t_envp_list	*prev_node;

	current_node = *(head);
	prev_node = *(head);
	if (ft_strncmp(current_node->key, key_str, \
	ft_strlen(current_node->key) + 1) == 0)
	{
		*(head) = current_node->next;
		destroy_envp_list_node(&current_node);
	}
	else
	{
		while (current_node != NULL)
		{
			if (ft_strncmp(current_node->key, key_str, \
			ft_strlen(current_node->key) + 1) == 0)
			{
				prev_node->next = current_node->next;
				destroy_envp_list_node(&current_node);
			}
			prev_node = current_node;
			current_node = current_node->next;
		}
	}
}

void	append_envp_node(t_envp_list **head, t_envp_list *node)
{
	t_envp_list	*tail;

	if ((*head) == NULL)
	{
		*head = node;
	}
	else
	{
		tail = (*head);
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		tail->next = node;
	}
}
