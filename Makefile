SRCS = Minishell.c parsing.c split.c strings.c error.c add_space.c qoutes.c \
		build.c expand.c cd.c echo.c env.c exec_helper.c execution.c exit.c export_helper1.c export_helpers.c export.c \
		libft.c linked_list.c pwd.c unset.c utils.c redirections.c helpers.c helpers1.c \
		heredoc.c expand_heredoc.c libc.c redir_helpers.c strings1.c helpers2.c helpers3.c helpers4.c signals.c ft_itoa.c
INC = minishell.h
NAME = minishell
CC = cc
READLINE_PREFIX = $(shell brew --prefix readline)
READLINE_INCLUDE = $(READLINE_PREFIX)/include
READLINE_LIB = $(READLINE_PREFIX)/lib
LIBS = -L$(READLINE_LIB) -lreadline -lncurses
INCLUDES = -I$(READLINE_INCLUDE)
FLAGS = -Wall -Wextra -Werror -fsanitize=address -g
OBJS = $(addprefix obj/, $(SRCS:.c=.o))

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${FLAGS} ${OBJS} ${LIBS} -o ${NAME}

obj/%.o: %.c ${INC}
	@mkdir -p $(dir $@)
	${CC} ${FLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -rf ${NAME}

re: fclean all

.PHONY: clean
