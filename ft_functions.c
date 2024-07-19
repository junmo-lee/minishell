/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choolee <choolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 20:45:49 by choolee           #+#    #+#             */
/*   Updated: 2024/07/17 21:35:06 by choolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}

int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

int	ft_isalnum(int c)
{
	if (((c >= '0') && (c <= '9')) \
		|| ((c >= 'a') && (c <= 'z')) \
		|| ((c >= 'A') && (c <= 'Z')) \
		|| c == '_')
		return (1);
	else
		return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	index;

	index = 0;
	while (s[index] != '\0')
		index ++;
	return (index);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	index;
	size_t	src_len;

	index = 0;
	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	while ((src[index] != '\0') && (index < (size - 1)))
	{
		dst[index] = src[index];
		index ++;
	}
	dst[index] = '\0';
	return (src_len);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	index;

	index = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dst_len >= size)
		return (src_len + size);
	while ((src[index] != '\0') && ((dst_len + index + 1) != size))
	{
		dst[dst_len + index] = src[index];
		index ++;
	}
	dst[dst_len + index] = '\0';
	return (dst_len + src_len);
}

char	*ft_strdup(const char *s1)
{
	char	*result;
	int		index;

	index = ft_strlen(s1);
	result = (char *)malloc(sizeof(char) * (index + 1));
	if (!result)
		return (0);
	ft_strlcpy(result, s1, index + 1);
	return (result);
}
