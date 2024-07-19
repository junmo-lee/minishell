/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:42:09 by junmlee           #+#    #+#             */
/*   Updated: 2023/10/25 15:12:08 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*arr;
	size_t			i;

	arr = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		*arr = c;
		arr++;
		i++;
	}
	return (b);
}
