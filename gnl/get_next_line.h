/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 21:01:15 by junmlee           #+#    #+#             */
/*   Updated: 2024/06/28 18:29:55 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_buf{
	char	*data;
	size_t	n_idx;
	size_t	len_data;
	ssize_t	ret_read;
	int		meet_n;
	int		end_flag;
}	t_buf;

void	*free_setnull(char **ptr);
void	update_buf(t_buf *buf);
size_t	gnl_strlen(const char *str);
void	gnl_strlcpy(char *dest, const char *src, size_t size);
char	*gnl_append(char *s1, char *s2, t_buf *buf);
char	*gnl_strndup(char *s1, size_t len);
int		read_line(t_buf *buf, char *buffer, int fd);
char	*make_ret(t_buf *buf);
char	*get_next_line(int fd);

#endif