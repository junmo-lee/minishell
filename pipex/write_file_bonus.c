/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_file_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:59:53 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/22 20:45:12 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	write_here_doc(t_vars *vars)
{
	char	*input;
	int		limiter;

	limiter = 1;
	while (limiter != 0)
	{
		input = readline("heredoc> ");
		if (input == NULL)
			continue ;
		if (ft_strncmp(vars->argv[2], input, ft_strlen(vars->argv[2])) == 0 \
			&& ft_strlen(input) == ft_strlen(vars->argv[2]) + 1)
		{
			limiter = 0;
			close(vars->here_doc_fd);
		}
		else
		{
			write(vars->here_doc_fd, input, ft_strlen(input));
			limiter = 1;
		}
		ft_memset(input, 0, ft_strlen(input));
		free(input);
	}
}

void	write_file(int *fd, char *file_path, int option)
{
	*fd = open(file_path, option, 0644);
	if (access(file_path, F_OK) == 0)
	{
		if (access(file_path, W_OK) == 0)
			return ;
		else
			write_stderr("Permission denied: ", file_path);
	}
	else
		write_stderr("No such file or directory: ", file_path);
}
