/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:05:02 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/27 16:17:21 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

char	**pipex_split(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
		{
			str[i++] = 1;
			while (str[i] != '\0' && str[i] != '\'')
				i++;
			str[i] = 1;
		}
		if (str[i] == '\"')
		{
			str[i++] = 1;
			while (str[i] != '\0' && str[i] != '\"')
				i++;
			str[i] = 1;
		}
		if (str[i] == c)
			str[i] = 1;
		i++;
	}
	return (ft_split(str, 1));
}
