/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:45:49 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/21 21:38:00 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*arr;

	if (s == NULL)
		return (NULL);
	if (ft_strlen(s) < start)
	{
		arr = (char *)malloc(sizeof(char));
		if (arr == NULL)
			return (NULL);
		*arr = '\0';
		return (arr);
	}
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (arr == NULL)
		return (NULL);
	ft_strlcpy(arr, (char *)s + start, len + 1);
	return (arr);
}
