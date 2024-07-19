.DEFAULT_GOAL := parse

NAME 	= .parse_relink
PARSE 	= parse
AR 		= ar

LIBFT 	= ./libft/libft.a
LIBFT_DIR = ./libft/

LDFLAGS	= -L$(LIBFT_DIR)
LDLIBS	= -lreadline -lft

CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror

SRCS 	= \
	main.c \
	parser.c token_list_funcs.c parser_list_funcs.c \
	tokenize_string_1.c tokenize_string_2.c \
	expand_env_vars_1.c expand_env_vars_2.c \
	combine_expanded_tokens.c \
	ft_functions.c

OBJS	= $(SRCS:.c=.o)
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