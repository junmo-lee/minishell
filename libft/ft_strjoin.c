/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:43:01 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/21 21:37:45 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	s1_len;
	unsigned int	s2_len;
	char			*arr;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	arr = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (arr == NULL)
		return (NULL);
	ft_strlcpy(arr, s1, s1_len + 1);
	ft_strlcpy(arr + s1_len, s2, s2_len + 1);
	arr[s1_len + s2_len] = '\0';
	return (arr);
}
