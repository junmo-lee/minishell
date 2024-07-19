/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:52:41 by junmlee           #+#    #+#             */
/*   Updated: 2023/10/19 16:52:46 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	find;

	find = (char)c;
	while (1)
	{
		if (*s == find)
			return ((char *)s);
		if (*s == '\0')
			return (NULL);
		s++;
	}
}
