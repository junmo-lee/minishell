/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:40:27 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:11:41 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../type_def.h"

// builtins
	// echo
int	option_check(const char *str);
int	echo(t_parser_list *cmd_list);
	// export
int	*builtin_export(t_parser_list *cmd_head, \
t_envp_list **envp_list);
int	check_key_syntax(char *key_str);
	// unset
int	unset(t_parser_list *cmd_head, t_envp_list **envp_list);
	// cd
int	cd(t_parser_list *cmd_list, t_envp_list **envp_list, char *pwd);
	// exit
int	builtin_exit(t_parser_list *head_next, t_status *status);

#endif