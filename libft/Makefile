.DEFAULT_GOAL := all

NAME	= libft.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror
CDEF	= -MMD -MP

AR		= ar
ARFLAGS	= -rcs

INC		= ./libft.h
SRCS	= ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
			ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c \
			ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
			ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
			ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c \
			ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
			ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c
SRCS_B	= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
			ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

OBJS	= $(SRCS:.c=.o)
OBJS_B	= $(SRCS_B:.c=.o)

DEF		= $(SRCS:.c=.d) $(SRCS_B:.c=.d)
-include $(DEP)

all: LAST_ALL
bonus: LAST_BONUS

clean :
	rm -f $(OBJS) $(OBJS_B) $(DEF)

fclean :
	rm -f $(OBJS) $(OBJS_B) $(DEF) $(NAME)

re :
	make fclean
	make all

$(NAME) :
	make all

%.o : %.c
	$(CC) $(CFLAGS) $(CDEF) -c $< -o $@ -I $(INC)

.PHONY : all bonus clean fclean re

LAST_ALL: $(OBJS)
	rm -f LAST_BONUS
	$(AR) $(ARFLAGS) $(NAME) $^
	touch $@

LAST_BONUS: $(OBJS) $(OBJS_B)
	rm -f LAST_ALL
	$(AR) $(ARFLAGS) $(NAME) $^
	touch $@