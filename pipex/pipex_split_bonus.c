/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:05:02 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/23 20:17:14 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	parse_name_args(t_cmd *cmd, char *cmd_argv)
{
	char	**temp;

	temp = pipex_split(cmd_argv, ' ');
	if (temp == NULL)
		exit(EXIT_FAILURE);
	if (*temp == NULL)
		write_stderr_exit("Command not found: ", NULL, 127);
	cmd->cmd_name = ft_strdup(*temp);
	cmd->args = temp;
	return (0);
}

char	**pipex_split(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
		{
			str[i++] = 1;
			while (str[i] != '\0' && str[i] != '\'')
				i++;
			str[i] = 1;
		}
		if (str[i] == '\"')
		{
			str[i++] = 1;
			while (str[i] != '\0' && str[i] != '\"')
				i++;
			str[i] = 1;
		}
		if (str[i] == c)
			str[i] = 1;
		i++;
	}
	return (ft_split(str, 1));
}
