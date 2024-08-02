/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:20:04 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/02 17:44:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_exit(t_parser_list *head_next, t_status *status)
{
	int	exit_val;

	if (head_next == NULL)
	{
		if (g_signal == MAIN_SIGINT)
			exit(EXIT_FAILURE);
		else
			exit((unsigned char)status->exit_status);
	}
	if (atoi_check_num(head_next->token, &exit_val))
	{
		if (head_next->next == NULL)
			exit((unsigned char)(exit_val));
		else
			status->exit_status = 1;
	}
	else
		exit(EXIT_NUMERIC);
	return (0);
}
