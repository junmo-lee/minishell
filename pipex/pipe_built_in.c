/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 15:42:24 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/30 15:17:40 by junmlee          ###   ########.fr       */
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
		write (vars->next_write, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1] == NULL && option_flag == 0)
			write(vars->next_write, "\n", 1);
		if (cmd->args[i + 1] != NULL)
			write(vars->next_write, " ", 1);
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
		perror(cmd->args[1]);
		exit(EXIT_FAILURE);
	}
	perror(cmd->args[1]);
	exit(EXIT_FAILURE);
}

void	pipe_pwd(t_vars *vars, t_cmd *cmd)
{
	cmd++;
	ft_putstr_fd(vars->pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
// main 의 export 에서 탐색만 하도록
void	pipe_export(t_vars *vars, t_cmd *cmd, t_status *status)
{
	int	i;
	char			*key_str;

	// fprintf(stderr, "export args : %d\n", strs_len(cmd->args));
	if (strs_len(cmd->args) == 1)
	{
		print_envp_in_export(status->env_list);
		exit(EXIT_SUCCESS);
	}
	i = 0;
	while (cmd->args[i] != NULL)
	{
		// fprintf(stderr, "export : %s\n", cmd->args[i]);
		key_str = get_key(cmd->args[i]); // malloc 
		if (check_key_syntax(key_str) == UNDEFINED_ERROR) // key규칙에 안 맞으면 에러메세지 띄어주고 다음 토큰으로 넘어 가는 듯
		{
			printf("\'%s\' : not a valid identifier\n", cmd->args[i]);
			free(key_str);
			i++;
			continue ;
		}
		if (ft_strncmp(key_str, "_", 2) == 0)
		{
			free(key_str);
			i++;
			continue ;
		}
		// if (find_equal(cmd->args[i]))
		// 	value_str = get_value(cmd->args[i]);
		// else
		// 	value_str = NULL;
		//insert_envp_node(status->env_list, key_str, value_str);
		i++;
	}
	vars++;
	exit(EXIT_SUCCESS);
}

void	pipe_unset(t_vars *vars, t_cmd *cmd, t_status *status)
{
	int	i;

	i = 0;
	while (cmd->args[i] != NULL)
	{
		if (check_key_syntax(cmd->args[i]) == UNDEFINED_ERROR)
			printf ("minishell: unset: \'%s\' : not a valid identifier", cmd->args[i]);
		i++;
	}
	status++;
	vars++;
	exit(EXIT_SUCCESS);
}

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
	{
		pipe_env_print_error(cmd->args[1]);
		return ;
	}
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

void	pipe_exit(t_vars *vars, t_cmd *cmd)
{
	unsigned char	exit_code;

	vars++;
	if (cmd->args[1] == NULL)
		exit(EXIT_SUCCESS);
	exit_code = ft_atoi(cmd->args[1]);
	exit(exit_code);
}