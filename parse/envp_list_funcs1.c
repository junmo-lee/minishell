/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_list_funcs1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 18:42:57 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	if (head == NULL || *(head) == NULL)
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
	t_envp_list	*node;
	t_envp_list	*prev_node;
	t_envp_list	*next_node;

	node = *(head);
	prev_node = *(head);
	if (ft_strncmp(node->key, key_str, ft_strlen(node->key) + 1) == 0)
	{
		*(head) = node->next;
		destroy_envp_list_node(&node);
	}
	else
	{
		while (node != NULL)
		{
			next_node = node->next;
			if (ft_strncmp(node->key, key_str, ft_strlen(node->key) + 1) == 0)
			{
				destroy_envp_list_node(&node);
				prev_node->next = next_node;
			}
			prev_node = node;
			node = next_node;
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
