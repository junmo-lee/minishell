/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 20:41:33 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:07:41 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return ((*(unsigned char *)s1 - *(unsigned char *)s2));
}

void	node_swap(t_envp_list *a, t_envp_list *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

void	bubble_sort(t_envp_list *head)
{
	int			is_swap;
	t_envp_list	*cur;
	t_envp_list	*last;

	last = NULL;
	if (head == NULL)
		return ;
	while (1)
	{
		is_swap = 0;
		cur = head;
		while (cur->next != last)
		{
			if (ft_strcmp(cur->key, cur->next->key) > 0)
			{
				node_swap(cur, cur->next);
				is_swap = 1;
			}
			cur = cur->next;
		}
		last = cur;
		if (is_swap == 0)
			break ;
	}
}
