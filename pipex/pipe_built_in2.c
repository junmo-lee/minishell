#include "../parser.h"

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

void	pipe_cd(t_status *status, t_vars *vars, t_cmd *cmd)
{
	char	*env_home;

	vars++;
	if (cmd->args[1] == NULL)
	{
		env_home = ft_getenv("HOME", status->env_list);
		if (env_home == NULL)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		cmd->args[1] = env_home;
	}
	if (access(cmd->args[1], F_OK) == 0)
	{
		if (access(cmd->args[1], X_OK) == 0)
		{
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd("cd: ", STDERR_FILENO);
		perror(cmd->args[1]);
		exit(EXIT_FAILURE);
	}
	else
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		perror(cmd->args[1]);
		exit(EXIT_FAILURE);
	}
}

void	pipe_pwd(t_vars *vars, t_cmd *cmd)
{
	cmd++;
	ft_putstr_fd(vars->pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}

void	pipe_exit(t_vars *vars, t_cmd *cmd)
{
	int	exit_code;

	vars++;
	if (cmd->args[1] == NULL)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
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
