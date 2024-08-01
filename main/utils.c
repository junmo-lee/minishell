/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:51:22 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:07:52 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_full_path(char *pwd, char *str)
{
	char	buf[PATH_MAX + 1];

	parse_dir(buf, pwd, str);
	write(STDERR_FILENO, buf, ft_strlen(buf));
	write_stderr_exit(NULL, ": Permission denied", EACCES_EXIT_CODE);
}
