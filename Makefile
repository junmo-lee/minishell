.DEFAULT_GOAL := minishell

NAME 	= .parse_relink
PARSE 	= minishell
AR 		= ar

LIBFT 	= libft/libft.a
LIBFT_DIR = libft/

LDFLAGS	= -L$(LIBFT_DIR)
LDLIBS	= -lreadline -lft

CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror -g3 -fsanitize=address

SRCS_PARSE	= \
	main.c \
	parser.c \
	parser_list_funcs.c parsed_tree_funcs.c token_list_funcs.c \
	check_syntax_funcs.c make_parsed_tree.c \
	envp_list_funcs1.c envp_list_funcs2.c \
	tokenize_string_1.c tokenize_string_2.c \
	expand_env_vars_1.c expand_env_vars_2.c \
	combine_expanded_tokens.c atoi_check_num.c \
	check_fd.c

SRCS_PIPE	= \
	pipex/find_cmd.c pipex/make_here_doc.c \
	pipex/do_pipex_bonus.c pipex/main_bonus.c \
	pipex/read_file_bonus.c  pipex/write_file_bonus.c \
	pipex/utils_bonus.c pipex/pipex_init_bonus.c \
	pipex/path_join_bonus.c pipex/pipex_split_bonus.c 

SRCS_GNL	= \
	gnl/get_next_line.c gnl/get_next_line_utils.c

SRCS_BUITIN	= \
	parse_dir_path.c pipex/pipe_built_in.c \
	builtins/echo.c builtins/export.c builtins/unset.c builtins/env.c \
	builtins/cd.c

OBJS	= $(SRCS_PARSE:.c=.o) $(SRCS_PIPE:.c=.o) $(SRCS_GNL:.c=.o) $(SRCS_BUITIN:.c=.o)
HEADER	= parser.h

ifdef DEBUG_FLAG
	CFLAGS += -g3 -fsanitize=address
endif

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

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