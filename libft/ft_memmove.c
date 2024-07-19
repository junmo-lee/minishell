/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 21:56:11 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/21 21:37:30 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const unsigned char	*src_uc = (unsigned char *)src;
	unsigned char		*dst_uc;
	size_t				i;

	dst_uc = (unsigned char *)dst;
	if (dst_uc == src_uc || len == 0)
		return (dst);
	i = 0;
	if (dst_uc < src_uc)
	{
		while (i < len)
		{
			dst_uc[i] = src_uc[i];
			i++;
		}
		return (dst);
	}
	while (i < len)
	{
		dst_uc[len - i - 1] = src_uc[len - i - 1];
		i++;
	}
	return (dst);
}
