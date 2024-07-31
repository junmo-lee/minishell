/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_file_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:59:53 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/31 19:08:05 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	meet_limiter(int here_doc_fd)
{
	close(here_doc_fd);
	return (1);
}

void	write_here_doc(int here_doc_fd, char *limiter)
{
	char	*input;
	int		is_end;

	is_end = 0;
	while (is_end != 1)
	{
		input = readline("> \001\033[s\002");
		if (input == NULL)
		{
			is_end = meet_limiter(here_doc_fd);
			return ;
		}
		if (ft_strcmp(limiter, input) == 0)
			is_end = meet_limiter(here_doc_fd);
		else
		{
			write(here_doc_fd, input, ft_strlen(input));
			write(here_doc_fd, "\n", 1);
			is_end = 0;
		}
		ft_memset(input, 0, ft_strlen(input));
		free(input);
	}
}

int	write_file(int *fd, char *file_path, int option)
{
	*fd = open(file_path, option, 0644);
	if (*fd == -1)
	{
		perror(file_path);
		return (-1);
	}
	if (access(file_path, F_OK) == 0)
	{
		if (access(file_path, W_OK) == 0)
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
