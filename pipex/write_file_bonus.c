/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_file_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:59:53 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/29 17:20:33 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	write_here_doc(int here_doc_fd, char *limiter)
{
	char	*input;
	int		meet_limiter;

	meet_limiter = 1;
	while (meet_limiter != 0)
	{
		meet_limiter++;
		// input = get_next_line(STDIN_FILENO);
		input = readline("> \001\033[s\002");
		if (input == NULL)
		{
			meet_limiter = 0;
			close(here_doc_fd);
			return ;
		}
		// //fprintf(stderr, "heredoc : [%s]\n", input);
		// input 이 limiter 과 완전히 같거나 빈 line에서 ctrl + d 시그널이 들어오면
		if (ft_strncmp(limiter, input, ft_strlen(limiter)) == 0 \
			&& ft_strlen(input) == ft_strlen(limiter))
		{
			meet_limiter = 0;
			close(here_doc_fd);
		}
		else
		{
			write(here_doc_fd, input, ft_strlen(input));
			write(here_doc_fd, "\n", 1);
			meet_limiter = 1;
		}
		ft_memset(input, 0, ft_strlen(input));
		free(input);
	}
}

int	write_file(int *fd, char *file_path, int option)
{
	*fd = open(file_path, option, 0644);
	if (access(file_path, F_OK) == 0)
	{
		if (access(file_path, W_OK) == 0)
			return (0);
		else
		{
			write_stderr("Permission denied: ", file_path);
			return (1);
		}
	}
	else
		write_stderr("No such file or directory: ", file_path);
	return (1);
}
