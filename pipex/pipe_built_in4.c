/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_built_in4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:05:48 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_env_print_error(char *str)
{
	ft_putstr_fd("env: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

void	pipe_env(t_vars *vars, t_cmd *cmd, t_status *status)
{
	int			i;
	t_envp_list	*current_node;

	current_node = status->env_list;
	i = 0;
	if (cmd->args[1] != NULL)
		return ;
	while (current_node != NULL)
	{
		if (current_node->value == NULL)
		{
			current_node = current_node->next;
			continue ;
		}
		write(1, current_node->key, ft_strlen(current_node->key));
		write(1, "=", 1);
		write(1, current_node->value, ft_strlen(current_node->value));
		write(1, "\n", 1);
		current_node = current_node->next;
	}
	vars++;
	exit(EXIT_SUCCESS);
}
