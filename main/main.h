#ifndef MAIN_H
# define MAIN_H

# include "../type_def.h"

void	update_pwd(char *pwd, char *str);
void	print_full_path(char *pwd, char *str);
int		atoi_check_num(const char *str, int *target);
void	bubble_sort(t_envp_list *start);
int		ft_strcmp(const char *s1, const char *s2);
void	parse_dir(char *target, char *pwd, char *str);
void	main_status_init(t_status *status, t_vars *vars, \
t_envp_list **envp_list, char **envp);
int		str_exist(char *str, t_status *status, t_vars *vars, \
t_envp_list *envp_list);
int		pipe_built_in(t_vars *vars, t_cmd *cmd, t_status *status);

#endif