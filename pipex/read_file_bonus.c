/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:58:59 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	read_file(int *fd, char *file_path, int option)
{
	*fd = open(file_path, option);
	if (*fd == -1)
	{
		perror(file_path);
		return (-1);
	}
	if (access(file_path, F_OK) == 0)
	{
		if (access(file_path, R_OK) == 0)
			return (0);
		else
		{
			perror(file_path);
			return (1);
		}
	}
	else
		perror(file_path);
	return (1);
}
