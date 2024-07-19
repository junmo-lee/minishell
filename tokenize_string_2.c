/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_string_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choolee <choolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:14:19 by choolee           #+#    #+#             */
/*   Updated: 2024/07/17 21:29:03 by choolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	tokenize_singlequote(t_info *info_s, char *str, int *index)
{
	*index += 1;
	while (str[*index] != '\'')
	{
		if (str[*index] == '\0')
		{
			info_s->error = ERROR;
			return ;
		}
		*index += 1;
	}
	info_s->type_code = SINGLEQUOTE;
	if (str[*index + 1] != ' ')
		info_s->connect_flag = 1;
}

static void	tokenize_doublequote(t_info *info_s, char *str, int *index)
{
	*index += 1;
	while (str[*index] != '\"')
	{
		if (str[*index] == '\0')
		{
			info_s->error = ERROR;
			return ;
		}
		*index += 1;
	}
	info_s->type_code = DOUBLEQUOTE;
	if (str[*index + 1] != ' ')
		info_s->connect_flag = 1;
}

void	find_last_index_of_token(t_info *info_s, char *str)
{
	int	index;

	index = info_s->start_index;
	while (str[index] != '\0')
	{
		if (str[index] == '\'')
			tokenize_singlequote(info_s, str, &index);
		else if (str[index] == '\"')
			tokenize_doublequote(info_s, str, &index);
		else if (str[index + 1] == ' ' || str[index + 1] == '<' \
		|| str[index + 1] == '>' || str[index + 1] == '|')
			info_s->connect_flag = 0;
		else if (str[index + 1] == '\'' || str[index + 1] == '\"')
			info_s->connect_flag = 1;
		else
		{
			index ++;
			continue ;
		}
		break ;
	}
	info_s->end_index = index;
}
