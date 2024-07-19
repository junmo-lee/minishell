/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:01:41 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/21 21:38:04 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char s, char const *set)
{
	if (set == NULL)
		return (1);
	while (*set != '\0')
	{
		if (s == *set)
			return (1);
		set++;
	}
	return (0);
}

static char	*strdup_len(const char *s1, unsigned int len)
{
	char			*arr;
	unsigned int	i;

	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (arr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		arr[i] = s1[i];
		i++;
	}
	*(arr + i) = '\0';
	return (arr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char			*arr;
	unsigned int	start;
	unsigned int	end;

	if (s1 == NULL || *s1 == '\0')
		return (strdup_len("", 0));
	start = 0;
	while (s1[start] != '\0' && is_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1) - 1;
	while (s1[end] != '\0' && is_in_set(s1[end], set) && start < end)
		end--;
	arr = strdup_len(s1 + start, end - start + 1);
	if (arr == NULL)
		return (NULL);
	return (arr);
}
