#include "../parser.h"

void	pipe_export_print(t_cmd *cmd, t_status *status)
{
	if (strs_len(cmd->args) == 1)
	{
		print_envp_in_export(status->env_list);
		exit(EXIT_SUCCESS);
	}
}

void	pipe_export_error(char *args)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(args, STDERR_FILENO);
	ft_putstr_fd(" : not a valid identifier\n", STDERR_FILENO);
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
			pipe_export_error(cmd->args[i]);
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
