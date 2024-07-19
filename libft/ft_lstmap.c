/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:01:08 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/21 21:38:25 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	add_back(t_list **lst, t_list *new, t_list **cache)
{
	t_list	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		if (*cache == NULL)
		{
			temp = *lst;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = new;
			*cache = new;
		}
		else
		{
			(*cache)->next = new;
			*(cache) = new;
		}
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*ret;
	t_list	*add_node;
	t_list	*cache;
	void	*current;

	if (f == NULL || del == NULL)
		return (NULL);
	ret = NULL;
	cache = NULL;
	while (lst != NULL)
	{
		current = f(lst->content);
		add_node = ft_lstnew(current);
		if (!add_node)
		{
			del(current);
			ft_lstclear(&ret, del);
			return (NULL);
		}
		add_back(&ret, add_node, &cache);
		lst = lst->next;
	}
	return (ret);
}
