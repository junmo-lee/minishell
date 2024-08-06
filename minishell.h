/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:04:11 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/05 18:22:14 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

int check_fd(char *str);

# include "libft/libft.h"
# include "pipex/pipex_bonus.h"
# include "gnl/get_next_line.h"
# include "parse/parser.h"
# include "builtins/builtin.h"
# include "main/main.h"

#endif