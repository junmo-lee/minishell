/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:58:43 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/22 20:51:09 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	import_path(t_vars *vars)
{
	char	*temp;
	int		i;

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
			return (0);
		}
		i++;
	}
	return (1);
}

void	init(t_vars *vars, int argc, char *argv[], char *envp[])
{
	ft_memset(vars, 0, sizeof(vars));
	vars->argc = argc;
	vars->argv = argv;
	vars->envp = envp;
	if (import_path(vars) == 1)
		exit(EXIT_FAILURE);
}

void	cmd_init(t_vars *vars, t_cmd *cmd, int count, char *cmd_argv)
{
	cmd->envp = vars->envp;
	cmd->cmd_argv = ft_strdup(cmd_argv);
	parse_name_args(cmd, cmd_argv);
	if (count != vars->cmd_len - 1)
	{
		if (pipe(vars->pipe_fd) == -1)
			exit(EXIT_FAILURE);
		vars->next_write = dup(vars->pipe_fd[1]);
		close(vars->pipe_fd[1]);
	}
	else
	{
		vars->next_write = dup(vars->file2_write_fd);
		close(vars->file2_write_fd);
	}
}
