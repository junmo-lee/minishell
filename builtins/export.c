/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 20:39:52 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/07 00:44:14 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_envp_in_export(t_envp_list *envp_list)
{
	while (envp_list != NULL)
	{
		write (1, envp_list->key, ft_strlen(envp_list->key));
		if (envp_list->value != NULL)
		{
			write (1, "=\"", 2);
			write (1, envp_list->value, ft_strlen(envp_list->value));
			write (1, "\"", 1);
		}
		write (1, "\n", 1);
		envp_list = envp_list->next;
	}
}

int	find_equal(char *str)
{
	while (*str != '\0')
	{
		if (*str == '=')
			return (1);
		str ++;
	}
	return (0);
}

int	check_key_syntax(char *key_str)
{
	int	index;

	if (key_str == NULL)
		return (UNDEFINED_ERROR);
	index = 0;
	if (ft_strlen(key_str) == 0)
		return (UNDEFINED_ERROR);
	if (ft_isdigit(key_str[index]) == 1)
		return (UNDEFINED_ERROR);
	while (key_str[index] != '\0')
	{
		if (ft_isalnum(key_str[index]) == 0 && key_str[index] != '_')
			return (UNDEFINED_ERROR);
		index ++;
	}
	return (0);
}

void	export_free_next_node(char *key_str, t_parser_list	**current_node)
{
	free(key_str);
	(*current_node) = (*current_node)->next;
}

void	builtin_export(t_parser_list *cmd_head, t_envp_list **envp_list)
{
	t_parser_list	*current_node;
	char			*key_str;
	char			*value_str;

	current_node = cmd_head->next;
	if (cmd_head->next != NULL)
	{
		while (current_node != NULL)
		{
			key_str = get_key(current_node->token);
			if (check_key_syntax(key_str) == UNDEFINED_ERROR)
				export_free_next_node(key_str, &current_node);
			else if (ft_strncmp(key_str, "_", 2) == 0)
				export_free_next_node(key_str, &current_node);
			else
			{
				if (find_equal(current_node->token))
					value_str = get_value(current_node->token);
				else
					value_str = NULL;
				insert_envp_node(envp_list, key_str, value_str);
				current_node = current_node->next;
			}
		}
	}
}
