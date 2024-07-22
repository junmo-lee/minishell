/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee <junmlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:04:54 by choolee           #+#    #+#             */
/*   Updated: 2024/07/22 19:45:49 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h> //malloc
# include <stdio.h> //printf , readline
# include <readline/readline.h> //readline
# include <readline/history.h> //add_history
# include <termios.h> // tcgetattr() 
# include <string.h> //strlen

# define UNDEFINED_ERROR -1
# define NO_ERROR 0
# define NOT_CLOSED_ERROR 1
# define MALLOC_ERROR 2
# define REDIRECTION_ERROR 3
# define PIPE_ERROR 4

# define STDIN_FILENO 0

typedef enum
{
	DEFAULT,
	CMD,
	STRING,
	EQUAL,
	REDIRECTION,
	HERE_DOC,
	PIPE,
	DOUBLEQUOTE,
	SINGLEQUOTE,
}	tag;


/*
t_token_list	p;

if (p.type == CMD)
 */

typedef struct s_parser_list
{
	// int inFd;
	// int outFd;
	// char *cmd;
	char					*token;
	tag						type;
	int						error;
	struct s_parser_list	*next;
}	t_parser_list;

typedef struct s_token_list
{
	char				*token;
	tag					type;
	int					error;
	int					connect_flag;
	struct s_token_list	*next;
}	t_token_list;

typedef struct s_info
{
	int	token_count;
	int	start_index;
	int	end_index;
	tag	type_code;
	int	connect_flag;
	int	error;
}	t_info;

typedef struct s_parsed_tree
{
	int						cmd_len;
	t_parser_list			*cmd_list_head;
	struct s_parsed_tree	*next;
	int						error;
}	t_parsed_tree;

// parser.c
t_parsed_tree	*parser(char *str);

// token_list_funcs.c
t_token_list	*create_token_node(t_info *info_s, char *str);
void			clear_token_list(t_token_list **head);
void			append_token_node(t_token_list **head, t_token_list *node);

// parser_list_funcs.c
t_parser_list	*create_parse_node(char *str, t_token_list *token_s);
void			destroy_parse_node(t_parser_list *node);
void			clear_parse_list(t_parser_list **head);
void			append_parse_node(t_parser_list **head, t_parser_list *node);

// parsed_tree_funcs.c
t_parsed_tree	*create_parsed_tree_node(int cmd_count, t_parser_list *cmd_list_head);
void			append_parsed_tree_node(t_parsed_tree **head, t_parsed_tree *node);

// tokenize_string_1.c
t_token_list	*tokenize_string(t_info *info_s, char *str);

// tokenize_string_2.c
void			find_last_index_of_token(t_info *info_s, char *str);

// expand_env_vars_1.c
void			expand_env_vars_in_token_list(t_token_list **token_s);

// expand_env_vars_2.c
char			*slice_string(int start_index, int end_index, char *str);
char			*get_envp_value(char *str, int *index);
char			*concatenate_strings(char *word1, char *word2);

// combine_expanded_tokens.c
t_parser_list	*combine_expanded_tokens(t_token_list **token_s);

// ft_functions.c
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
size_t			ft_strlen(const char *s);
char			*ft_strdup(const char *s1);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
size_t			ft_strlcat(char *dst, const char *src, size_t size);

#endif