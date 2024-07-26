/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:56:28 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/26 17:20:04 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../type_def.h"
# include "../libft/libft.h"

# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <limits.h>

char	*path_join(char const *s1, char const *s2);
void	init(t_vars *vars, int argc, char *argv[], char *envp[]);
void	cmd_init(t_vars *vars, t_cmd *cmd, int count, char *cmd_argv);

void	do_pipex_bonus(t_vars *vars, t_cmd *cmd);
void	check_cmd_access(t_vars *vars, t_cmd *cmd);

void	child(t_vars *vars, t_cmd *cmd);
int		write_stderr(char *error_msg, char *name);
void	write_stderr_exit(char *error_msg, char *name, int errorcode);
char	**pipex_split(char *str, char c);
int		parse_name_args(t_cmd *cmd, char *cmd_argv);

void	read_file(int *fd, char *file_path, int option);

void	write_file(int *fd, char *file_path, int option);
void	write_here_doc(int here_doc_fd, char *limiter);

int		free_strs(char *strs[], int exit_code);
void	free_cmds(t_cmd *cmd);

void	wait_processes(t_vars *vars, t_cmd *cmd);
int		main_return(t_vars *vars, t_cmd *cmd);
int		run_cmd_tree(t_status *status, t_parsed_tree *tree);

#endif