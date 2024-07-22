/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:56:28 by junmlee           #+#    #+#             */
/*   Updated: 2024/07/22 20:46:18 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"

# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <limits.h>

typedef struct s_cmd
{
	pid_t	pid;
	char	*cmd_path;
	char	*cmd_name;
	char	**args;
	char	**envp;
	char	*cmd_argv;
	int		cant_execute;
	int		status;
	int		is_end;
}	t_cmd;

typedef struct s_vars
{
	int				argc;
	char			**argv;
	char			**envp;
	char			**path;
	int				file1_read_fd;
	int				file2_write_fd;
	int				here_doc_fd;
	int				pipe_fd[2];
	int				prev_read;
	int				next_write;
	int				cmd_len;
	int				is_here_doc;
	char			*pwd;
}	t_vars;

//main에서 받아올때 필요
typedef struct s_parsed_tree
{
	int						cmd_len;
	t_parser_list			*cmd_list_head;
	struct s_parsed_tree	*next;
	int						error;
}	t_parsed_tree;

char	*path_join(char const *s1, char const *s2);
void	init(t_vars *vars, int argc, char *argv[], char *envp[]);
void	cmd_init(t_vars *vars, t_cmd *cmd, int count, char *cmd_argv);

void	do_pipex_bonus(t_vars *vars, t_cmd *cmd);
void	check_cmd_access(t_vars *vars, t_cmd *cmd);

int		write_stderr(char *error_msg, char *name);
void	write_stderr_exit(char *error_msg, char *name, int errorcode);
char	**pipex_split(char *str, char c);
int		parse_name_args(t_cmd *cmd, char *cmd_argv);

void	read_file(int *fd, char *file_path, int option);

void	write_file(int *fd, char *file_path, int option);
void	write_here_doc(t_vars *vars);

int		free_strs(char *strs[], int exit_code);
void	free_cmds(t_cmd *cmd);

void	wait_processes(t_vars *vars, t_cmd *cmd);
int		main_return(t_vars *vars, t_cmd *cmd);
int		run_cmd_tree(t_vars *vars, t_parsed_tree *tree);

#endif