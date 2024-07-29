#ifndef PARSER_H
# define PARSER_H

# include "type_def.h"

# include "libft/libft.h"
# include "pipex/pipex_bonus.h"
# include "gnl/get_next_line.h"

# include <stdlib.h> 
# include <stdio.h> 
# include <readline/readline.h> 
# include <readline/history.h> 
# include <termios.h> 
# include <string.h> 


	
t_parsed_tree	*parser(char *str, t_status *status, t_envp_list *envp_list);

	
t_token_list	*create_token_node(t_info *info_s, char *str);
void			clear_token_list(t_token_list **head);
void			append_token_node(t_token_list **head, t_token_list *node);

	
t_parser_list	*create_parse_node(char *str, t_token_list *token_s);
void			destroy_parse_node(t_parser_list *node);
void			clear_parse_list(t_parser_list **head);
void			append_parse_node(t_parser_list **head, t_parser_list *node);

	
t_envp_list		*create_envp_node(char *key_str, char *value_str);
void			clear_envp_list(t_envp_list **head);
void			remove_node_by_key(t_envp_list **head, char *key_str);
void			append_envp_node(t_envp_list **head, t_envp_list *node);

	
int				insert_envp_node(t_envp_list **head, char *key_str, char *value_str);
t_envp_list		*get_envp(char **envp);
char			*get_key(char *str);
char			*get_value(char *str);
char			*ft_getenv(char *key_str, t_envp_list *envp_list);

	
t_parsed_tree	*create_parsed_tree_node(int cmd_count, t_parser_list *cmd_list_head);
void			append_parsed_tree_node(t_parsed_tree **head, t_parsed_tree *node);
void			clear_parsed_tree(t_parsed_tree **head);

	
void			check_syntax(t_info *info_s, t_parser_list *parse_list);

	
t_parsed_tree	*make_parsed_tree(t_parser_list *parser_list);

	
t_token_list	*tokenize_string(t_info *info_s, char *str);

	
void			find_last_index_of_token(t_info *info_s, char *str);

	
void			expand_env_vars_in_token_list(t_token_list **token_s, t_status *status, t_envp_list *envp_list, t_info *s_info);
char			*expand_env_vars(char *token, t_status *status, t_envp_list *envp_list, t_info *s_info);

	
char			*slice_string(int start_index, int end_index, char *str);
char			*get_envp_value(char *str, int *index, t_envp_list *envp_list);
char			*concat_strs(char *word1, char *word2);
void			handle_dilimiter_with_env(t_token_list **token_list);

	
t_parser_list	*combine_expanded_tokens(t_token_list **token_s);


	
int				option_check(const char *str);
int				echo(t_parser_list *cmd_list);
	
int				*export(t_parser_list *cmd_head, t_envp_list **envp_list);
int				check_key_syntax(char *key_str);
	
int				unset(t_parser_list *cmd_head, t_envp_list **envp_list);
	
int				env(t_parser_list *cmd_list, t_envp_list **envp_list);

	
int 			cd(t_parser_list *cmd_list, t_envp_list **envp_list, char *pwd);


int check_fd(char *str);

void	update_pwd(char *pwd, char *str);

void	print_full_path(char *pwd, char *str);

int	atoi_check_num(const char *str, int *target);

#endif