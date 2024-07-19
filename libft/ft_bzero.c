/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:57:07 by junmlee           #+#    #+#             */
/*   Updated: 2023/10/25 15:11:12 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *b, size_t len)
{
	unsigned char	*data;
	size_t			i;

	data = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		*data = 0;
		data++;
		i++;
	}
	return ;
}
