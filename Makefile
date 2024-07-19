.DEFAULT_GOAL := parse

NAME 	= .parse_relink
PARSE 	= parse
AR 		= ar
CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror
LDLIBS	= -lreadline

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

all: $(NAME)

$(NAME) : $(PARSE)
	touch $@

$(PARSE) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(PARSE)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME) $(PARSE)

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re