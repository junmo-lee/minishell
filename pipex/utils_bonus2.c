/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:13:39 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	stdin_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = SIGINT;
		write(STDOUT_FILENO, "^C\n", 4);
	}
	if (signo == SIGQUIT)
	{
		g_signal = SIGQUIT;
		write(STDOUT_FILENO, "^\\Quit: 3\n", 11);
	}
}

int	close_all_fd(t_vars *vars, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < vars->cmd_len)
	{
		if ((cmd + i)->redirection_in != -1)
			close((cmd + i)->redirection_in);
		if ((cmd + i)->redirection_out != -1)
			close((cmd + i)->redirection_out);
	}
	return (0);
}

int	get_exit_status(int status)
{
	return (((*(int *)&(status)) >> 8) & 0x000000ff);
}
