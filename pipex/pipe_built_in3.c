#include "../parser.h"

void	pipe_export_print(t_cmd *cmd, t_status *status)
{
	if (strs_len(cmd->args) == 1)
	{
		print_envp_in_export(status->env_list);
		exit(EXIT_SUCCESS);
	}
}

void	pipe_export(t_cmd *cmd, t_status *status)
{
	int		i;
	char	*key_str;

	pipe_export_print(cmd, status);
	i = 0;
	while (cmd->args[i] != NULL)
	{
		key_str = get_key(cmd->args[i]);
		if (check_key_syntax(key_str) == UNDEFINED_ERROR)
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
		i++;
	}
	exit(EXIT_SUCCESS);
}

void	pipe_unset(t_vars *vars, t_cmd *cmd, t_status *status)
{
	int	i;

	i = 0;
	while (cmd->args[i] != NULL)
	{
		if (check_key_syntax(cmd->args[i]) == UNDEFINED_ERROR)
			printf("minishell: unset: \'%s\' : not a valid identifier", \
				cmd->args[i]);
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
