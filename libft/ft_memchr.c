/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:56:25 by junmlee           #+#    #+#             */
/*   Updated: 2023/10/25 15:11:46 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_uc;
	unsigned char	target;
	size_t			i;

	target = (unsigned char)c;
	s_uc = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (*s_uc == target)
			return (s_uc);
		s_uc++;
		i++;
	}
	return (0);
}
