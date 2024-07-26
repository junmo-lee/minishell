/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 20:51:02 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/28 18:30:45 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*free_setnull(char **ptr)
{
	void	*temp;

	temp = *ptr;
	*ptr = NULL;
	free(temp);
	return (NULL);
}

size_t	gnl_strlen(const char *str)
{
	size_t	i;

	if (str == NULL)
		return (-1);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	gnl_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (size == 0)
		return ;
	i = 0;
	while (*(src + i) != '\0' && (i + 1) < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

char	*gnl_append(char *s1, char *s2, t_buf *buf)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*arr;

	s2_len = gnl_strlen(s2);
	if (s1 == NULL)
	{
		free_setnull(&(s1));
		return (gnl_strndup(s2, s2_len));
	}
	s1_len = buf->len_data;
	arr = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (arr == NULL)
	{
		free_setnull(&(s1));
		return (NULL);
	}
	gnl_strlcpy(arr, s1, s1_len + 1);
	gnl_strlcpy(arr + s1_len, s2, s2_len + 1);
	arr[s1_len + s2_len] = '\0';
	free_setnull(&(s1));
	return (arr);
}

char	*gnl_strndup(char *s1, size_t len)
{
	size_t	i;
	char	*arr;

	if (s1 == NULL || len == 0)
		return (NULL);
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (arr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		arr[i] = s1[i];
		i++;
	}
	arr[i] = '\0';
	return (arr);
}
