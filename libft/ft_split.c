/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 18:35:08 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/21 21:38:18 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_ret_null(char **arr)
{
	unsigned int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

static char	*_add_word(const char *s1, char c)
{
	unsigned int	i;
	unsigned int	len;
	char			*arr;

	len = 0;
	while (s1[len] != '\0' && s1[len] != c)
		len++;
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

static char	**arr_init(char const *s, char c)
{
	unsigned int	i;
	unsigned int	cnt;
	char			**ret;

	if (s == NULL)
		return (NULL);
	i = 0;
	cnt = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			cnt++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
	}
	ret = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (ret == NULL)
		return (NULL);
	return (ret);
}

char	**ft_split(char const *s, char c)
{
	char			**arr;
	unsigned int	i;
	unsigned int	cnt;

	arr = arr_init(s, c);
	if (arr == NULL)
		return (NULL);
	i = 0;
	cnt = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			arr[cnt] = _add_word(s + i, c);
			if (arr[cnt] == NULL)
				return (free_ret_null(arr));
			while (s[i] != '\0' && s[i] != c)
				i++;
			cnt++;
		}
	}
	arr[cnt] = NULL;
	return (arr);
}
