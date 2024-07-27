/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:56:28 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/27 16:51:12 by junmlee          ###   ########.fr       */
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

//디버그용
# include <stdio.h>

char	*path_join(char const *s1, char const *s2);
void	init(t_vars *vars, int argc, char *argv[], char *envp[]);

int		pipe_built_in(t_vars *var, t_cmd *cmd);
int		check_cmd(t_vars *vars, t_cmd *cmd);
int		check_cmd_path(t_vars *vars, t_cmd *cmd);

void	pipe_echo(t_vars *vars, t_cmd *cmd);
void	pipe_cd(t_vars *vars, t_cmd *cmd);
void	pipe_pwd(t_vars *vars, t_cmd *cmd);
void	pipe_export(t_vars *vars, t_cmd *cmd);
void	pipe_unset(t_vars *vars, t_cmd *cmd);
void	pipe_env(t_vars *vars, t_cmd *cmd);
void	pipe_exit(t_vars *vars, t_cmd *cmd);


void	child(t_vars *vars, t_cmd *cmd);
int		write_stderr(char *error_msg, char *name);
void	write_stderr_exit(char *error_msg, char *name, int errorcode);
char	**pipex_split(char *str, char c);

void	read_file(int *fd, char *file_path, int option);

void	write_file(int *fd, char *file_path, int option);
void	write_here_doc(int here_doc_fd, char *limiter);

int		free_strs(char *strs[], int exit_code);
void	free_cmds(t_cmd *cmd);

void	wait_processes(t_vars *vars, t_cmd *cmd);
int		run_cmd_tree(t_status *status, t_parsed_tree *tree);

#endif