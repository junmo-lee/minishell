#include "../parser.h"

void	change_directory(t_envp_list **envp_list, char *pwd, char *str)
{
	if (access(str, X_OK) == 0)
	{
		insert_envp_node(envp_list, ft_strdup("OLDPWD"), ft_strdup(pwd));
		update_pwd(pwd, str);
		chdir(pwd);
		insert_envp_node(envp_list, ft_strdup("PWD"), ft_strdup(pwd));
	}
}

int	cd(t_parser_list *cmd_list, t_envp_list **envp_list, char *pwd)
{
	char	*env_home;

	if (cmd_list->next == NULL)
	{
		env_home = ft_getenv("HOME", *envp_list);
		if (env_home == NULL)
			return (EXIT_FAILURE);
		change_directory(envp_list, pwd, env_home);
		return (EXIT_SUCCESS);
	}
	if (cmd_list->next->token == NULL)
		return (EXIT_FAILURE);
	if (access(cmd_list->next->token, X_OK) == 0)
		change_directory(envp_list, pwd, cmd_list->next->token);
	return (EXIT_SUCCESS);
}
