/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:58:43 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	import_path(t_vars *vars)
{
	char	*temp;
	int		i;

	if (vars->envp == NULL)
	{
		vars->path = ft_split("", ':');
		return ;
	}
	i = 0;
	while (vars->envp[i] != NULL)
	{
		if (ft_strncmp(vars->envp[i], "PATH=", 5) == 0)
		{
			vars->path = ft_split(vars->envp[i], ':');
			if (vars->path == NULL)
				exit(EXIT_FAILURE);
			temp = vars->path[0];
			vars->path[0] = ft_substr(temp, 5, ft_strlen(temp));
			ft_memset(temp, 0, ft_strlen(temp));
			free(temp);
			return ;
		}
		i++;
	}
	vars->path = ft_split("", ':');
}

char	*make_str_env(char *key, char *value)
{
	char			*ret;
	const int		key_len = ft_strlen(key);
	const int		value_len = ft_strlen(value);

	ret = ft_calloc(key_len + value_len + 2, sizeof(char));
	ft_strlcat(ret, key, key_len + value_len + 2);
	ret[key_len] = '=';
	ft_strlcat(ret, value, key_len + value_len + 2);
	return (ret);
}

void	main_init_envp_parse(t_vars *vars, t_envp_list *env_list)
{
	t_envp_list	*current_node;
	int			envp_len;

	envp_len = 0;
	current_node = env_list;
	while (current_node != NULL)
	{
		if (current_node->value != NULL)
			envp_len++;
		current_node = current_node->next;
	}
	vars->envp = malloc(sizeof(char *) * (envp_len + 1));
	if (vars->envp == NULL)
		exit(EXIT_FAILURE);
	envp_len = 0;
	current_node = env_list;
	while (current_node != NULL)
	{
		if (current_node->value != NULL)
			vars->envp[envp_len++] = make_str_env(current_node->key, \
				current_node->value);
		current_node = current_node->next;
	}
	vars->envp[envp_len] = NULL;
}

void	vars_init(t_vars *vars, t_envp_list *env_list)
{
	ft_memset(vars, 0, sizeof(vars));
	if (env_list == NULL)
		vars->envp = NULL;
	else
		main_init_envp_parse(vars, env_list);
	import_path(vars);
}
