.DEFAULT_GOAL := minishell

NAME 	= .parse_relink
PARSE 	= minishell

LIBFT 	= libft/libft.a
LIBFT_DIR = libft/

BREW_PREFIX := $(shell brew --prefix)

LDFLAGS	= -L$(LIBFT_DIR) -L$(BREW_PREFIX)/opt/readline/lib
LDLIBS	= -lft -lreadline 

INC		= -I$(BREW_PREFIX)/opt/readline/include

CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror

PARSE_DIR = ./parse/

SRCS_PARSE	= \
	$(PARSE_DIR)parser.c \
	$(PARSE_DIR)parser_list_funcs.c $(PARSE_DIR)parsed_tree_funcs.c $(PARSE_DIR)token_list_funcs.c \
	$(PARSE_DIR)check_syntax_funcs.c $(PARSE_DIR)make_parsed_tree.c \
	$(PARSE_DIR)envp_list_funcs1.c $(PARSE_DIR)envp_list_funcs2.c \
	$(PARSE_DIR)tokenize_string_1.c $(PARSE_DIR)tokenize_string_2.c \
	$(PARSE_DIR)expand_env_vars_1.c $(PARSE_DIR)expand_env_vars_2.c \
	$(PARSE_DIR)combine_expanded_tokens.c \

SRCS_PIPE	= \
	pipex/find_cmd.c pipex/make_here_doc.c \
	pipex/do_pipex_bonus.c pipex/main_bonus.c \
	pipex/pipe_tree_parse.c pipex/pipe_node_parse.c \
	pipex/read_file_bonus.c  pipex/write_file_bonus.c \
	pipex/utils_bonus.c pipex/utils_bonus2.c \
	pipex/pipex_init_bonus.c pipex/pipe_fork.c \
	pipex/path_join_bonus.c pipex/pipex_split_bonus.c 

SRCS_GNL	= \
	gnl/get_next_line.c gnl/get_next_line_utils.c

SRCS_BUITIN	= \
	pipex/pipe_built_in.c pipex/pipe_built_in2.c \
	pipex/pipe_built_in3.c pipex/pipe_built_in4.c \
	builtins/echo.c builtins/export.c builtins/unset.c \
	builtins/cd.c builtins/sort_env.c builtins/builtin_exit.c

SRCS_MAIN = \
	main/main.c main/main_splited.c \
	main/parse_dir_path.c main/utils.c main/atoi_check_num.c

OBJS	= $(SRCS_PARSE:.c=.o) $(SRCS_PIPE:.c=.o) $(SRCS_GNL:.c=.o) $(SRCS_BUITIN:.c=.o) $(SRCS_MAIN:.c=.o) 

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT) :
	make -C $(LIBFT_DIR) all

all: $(NAME)

$(NAME) : $(PARSE)
	touch $@

$(PARSE) : $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $(PARSE)

clean :
	rm -rf $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean :
	rm -rf $(OBJS) $(NAME) $(PARSE)
	make -C $(LIBFT_DIR) fclean

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re