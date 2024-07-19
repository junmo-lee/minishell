/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 15:02:26 by junmlee           #+#    #+#             */
/*   Updated: 2023/10/25 15:11:59 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*dst_u;
	unsigned char	*cur;
	size_t			i;

	if (dst == src)
		return (NULL);
	dst_u = (unsigned char *)dst;
	cur = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		dst_u[i] = cur[i];
		i++;
	}
	return (dst);
}
