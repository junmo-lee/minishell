/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_built_in2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:38:37 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/06 21:07:25 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_echo(t_cmd *cmd)
{
	int	option_flag;
	int	i;
	int	print_flag;

	print_flag = 0;
	i = 1;
	option_flag = 0;
	while (cmd->args[i] != NULL)
	{
		if (option_check(cmd->args[i]) == 1 && print_flag == 0)
		{
			option_flag = 1;
			i++;
			continue ;
		}
		print_flag = 1;
		write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1] == NULL && option_flag == 0)
			write(STDOUT_FILENO, "\n", 1);
		if (cmd->args[i + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	exit(EXIT_SUCCESS);
}

void	pipe_exit(t_cmd *cmd, t_status *status)
{
	int	exit_code;

	if (cmd->args[1] == NULL)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit((unsigned char)status->exit_status);
	}
	else if (atoi_check_num(cmd->args[1], &exit_code))
	{
		if (cmd->args[2] == NULL)
			exit((unsigned char)(exit_code));
		else
		{
			ft_putstr_fd("exit\nexit: too many arguments\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		ft_putstr_fd("exit\nexit: a: numeric argument required\n", STDERR_FILENO);
		exit(EXIT_NUMERIC);
	}
}

void	pipe_pwd(t_vars *vars)
{
	ft_putstr_fd(vars->pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
