#ifndef TYPE_DEF_H
#  define TYPE_DEF_H

# define UNDEFINED_ERROR -1
# define NO_ERROR 0

# define NOT_CLOSED_ERROR 1
# define MALLOC_ERROR 2
# define REDIRECTION_ERROR 3
# define PIPE_ERROR 4

# define STATUS_SUCCESS 0x0
# define STATUS_FAILURE 0x0000100

# define SIGINT_EXIT_CODE 130
# define SIGQUIT_EXIT_CODE 131

# include <unistd.h>
# include <sys/syslimits.h>
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

typedef struct s_parser_list
{
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
	int		index;
	int		str_len;
	int		token_count;
	int		start_index;
	int		end_index;
	tag		type_code;
	int		connect_flag;
	int		first_text_idx;
	char	*expanded_token;
	int		error;
}	t_info;

typedef struct s_parsed_tree
{
	int						cmd_len;
	t_parser_list			*cmd_list_head;
	struct s_parsed_tree	*next;
	int						error;
	int						arg_len;
}	t_parsed_tree;

typedef struct s_envp_list
{
	char				*key;
	char				*value;
	struct s_envp_list	*next;
}	t_envp_list;

// pipex type 추가
typedef struct s_cmd
{
	pid_t	pid;
	char	*cmd_path;
	char	*cmd_name;
	char	**args;
	char	**envp;
	int		redirection_in;
	int		redirection_out;
	int		is_exist;
	int		status;
	int		is_end;
	int		is_built_in;
}	t_cmd;

typedef struct s_vars
{
	int				argc;
	char			**argv;
	char			**envp;
	char			**path;
	int				here_doc_fd;
	int				pipe_fd[2];
	int				prev_read;
	int				next_write;
	int				cmd_len;
	char			*pwd;
}	t_vars;

typedef struct s_status
{
	int			exit_status;
	t_vars		*one_line;
	int			is_here_doc;
	int			here_doc_fd;
	char		*temp_here_doc;
	char		pwd[PATH_MAX + 1];
	t_envp_list	*env_list;
}	t_status;

#endif