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

void	print_full_path(char *pwd, char *str);
char	*path_join(char const *s1, char const *s2);
void	init(t_vars *vars, t_envp_list *env_list);

int		pipe_built_in(t_vars *var, t_cmd *cmd, t_status *status);
int		check_cmd(t_status *status, t_vars *vars, t_cmd *cmd);
int		check_cmd_path(t_status *status, t_vars *vars, t_cmd *cmd);

int		strs_len(char **strs);

void	pipe_echo(t_vars *vars, t_cmd *cmd);
void	pipe_cd(t_vars *vars, t_cmd *cmd);
void	pipe_pwd(t_vars *vars, t_cmd *cmd);

void			print_envp_in_export(t_envp_list *envp_list);
void	pipe_export(t_vars *vars, t_cmd *cmd, t_status *status);
void	pipe_unset(t_vars *vars, t_cmd *cmd, t_status *status);
void	pipe_env(t_vars *vars, t_cmd *cmd, t_status *stats);
void	pipe_exit(t_vars *vars, t_cmd *cmd);

int		make_here_doc(t_vars *vars, t_cmd *cmd, char *token);

void	child(t_vars *vars, t_cmd *cmd, t_status *status);
int		write_stderr(char *error_msg, char *name);
void	write_stderr_exit(char *name, char *error_msg, int errorcode);
char	**pipex_split(char *str, char c);

int		read_file(int *fd, char *file_path, int option);

int		write_file(int *fd, char *file_path, int option);
void	write_here_doc(int here_doc_fd, char *limiter);

int		free_strs(char **strs, int exit_code);
void	free_cmds(t_cmd *cmd);
int		dup_close(int *dst, int *src_close);

void	signal_handler(int signo);
void	stdin_handler(int signo);
int		wait_processes(t_vars *vars, t_cmd *cmd);
int		run_cmd_tree(t_status *status, t_parsed_tree *tree);
int		tree_parse(t_parsed_tree *tree, t_vars *vars, t_cmd *cmd);
int		list_parse(t_vars *vars, t_cmd *cmd, t_parsed_tree *current_node);
int		node_parse(t_vars *vars, t_cmd *cmd, t_parser_list	**cur, int *arg_index);

#endif