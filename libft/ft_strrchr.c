/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 22:23:31 by junmlee           #+#    #+#             */
/*   Updated: 2023/10/19 16:50:59 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ret;
	char	find;

	find = (char)c;
	ret = NULL;
	while (1)
	{
		if (*s == find)
			ret = (char *)s;
		if (*s == '\0')
			return (ret);
		s++;
	}
}
