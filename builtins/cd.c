/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:04:11 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/05 17:58:39 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_directory(t_envp_list **envp_list, char *pwd, char *str)
{
	struct stat	dir_stat;
	char		buf[PATH_MAX];

	ft_strlcpy(buf, pwd, PATH_MAX);
	if (stat(str, &dir_stat) == 0)
	{
		if (S_ISDIR(dir_stat.st_mode) && (access(str, X_OK) == 0))
		{
			update_pwd(buf, str);
			if (chdir(buf) == 0)
			{
				insert_envp_node(envp_list, \
					ft_strdup("OLDPWD"), ft_strdup(pwd));
				insert_envp_node(envp_list, ft_strdup("PWD"), ft_strdup(buf));
				ft_strlcpy(pwd, buf, PATH_MAX);
			}
		}
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
