/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dir_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 22:27:48 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	strs_len(char **strs)
{
	int	len;

	len = 0;
	while (strs[len] != NULL)
		len++;
	return (len);
}

void	parse_relative_up_dir(char *target)
{
	int	target_len;

	target_len = ft_strlen(target);
	while (target_len > 0 && target[target_len] != '/')
	{
		target[target_len] = '\0';
		target_len--;
	}
	if (ft_strcmp(target, "/") != 0)
		target[target_len] = '\0';
}

void	parse_relative_dir(char *target, char *str)
{
	int		i;
	char	**str_part;

	str_part = ft_split(str, '/');
	i = 0;
	while (str_part[i] != NULL)
	{
		if (str_part[i][0] == '\0' || ft_strcmp(str_part[i], ".") == 0)
			(void)i;
		else if (ft_strcmp(str_part[i], "..") == 0)
		{
			if (ft_strcmp(target, "/") != 0)
				parse_relative_up_dir(target);
		}
		else
		{
			if (ft_strcmp(target, "/") != 0)
				target[ft_strlen(target)] = '/';
			ft_strlcat(target, str_part[i], PATH_MAX);
		}
		i++;
	}
	free_strs(str_part, EXIT_SUCCESS);
}

void	parse_dir(char *target, char *pwd, char *str)
{
	ft_memset(target, 0, PATH_MAX);
	if (str == NULL)
		ft_strlcpy(target, pwd, PATH_MAX);
	else if (str[0] == '/')
	{
		ft_strlcpy(target, str, PATH_MAX);
		if (ft_strcmp(target, "/") != 0)
		{
			if (target[ft_strlen(target) - 1] == '/')
				target[ft_strlen(target) - 1] = '\0';
		}
	}
	else
	{
		ft_strlcpy(target, pwd, PATH_MAX);
		parse_relative_dir(target, str);
	}
}

void	update_pwd(char *pwd, char *str)
{
	char	buf[PATH_MAX + 1];

	parse_dir(buf, pwd, str);
	ft_memset(pwd, 0, PATH_MAX);
	ft_strlcpy(pwd, buf, PATH_MAX);
}
