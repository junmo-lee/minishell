/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 15:42:24 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/28 16:52:00 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

/*
					if (ft_strncmp(head->cmd_list_head->token, "export", 7) == 0)
						export(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "unset", 6) == 0)
						unset(head->cmd_list_head, &envp_list);
					else if (ft_strncmp(head->cmd_list_head->token, "env", 4) == 0)
						env(head->cmd_list_head, &envp_list);
*/
void	pipe_echo(t_vars *vars, t_cmd *cmd)
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
		write (STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1] == NULL && option_flag == 0)
			write(STDOUT_FILENO, "\n", 1);
		if (cmd->args[i + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	(void)vars;
	exit(EXIT_SUCCESS);
}
void	pipe_cd(t_vars *vars, t_cmd *cmd)
{
	vars++;
	if (cmd->args[1] == NULL)
		exit(EXIT_SUCCESS);
	if (access(cmd->args[1], F_OK) == 0)
	{
		if (access(cmd->args[1], X_OK) == 0)
		{
			exit(EXIT_SUCCESS);
		}
		write_stderr("Permission denied: ", cmd->args[1]);
		exit(EXIT_FAILURE);
	}
	write_stderr("No such file or directory: ", cmd->args[1]);
	exit(EXIT_FAILURE);
}

void	pipe_pwd(t_vars *vars, t_cmd *cmd)
{
	cmd++;
	ft_putstr_fd(vars->pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}

void	pipe_export(t_vars *vars, t_cmd *cmd)
{
	vars++;
	cmd++;
	exit(EXIT_SUCCESS);
}

void	pipe_unset(t_vars *vars, t_cmd *cmd)
{
	vars++;
	cmd++;
	exit(EXIT_SUCCESS);
}

void	pipe_env(t_vars *vars, t_cmd *cmd)
{
	vars++;
	cmd++;
	exit(EXIT_SUCCESS);
}

void	pipe_exit(t_vars *vars, t_cmd *cmd)
{
	unsigned char	exit_code;

	vars++;
	if (cmd->args[1] == NULL)
		exit(EXIT_SUCCESS);
	exit_code = ft_atoi(cmd->args[1]);
	exit(exit_code);
}