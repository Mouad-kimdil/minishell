SRCS = Minishell.c parsing.c split.c strings.c error.c add_space.c qoutes.c \
		build.c expand.c cd.c echo.c env.c exec_helper.c execution.c exit.c export_helper1.c export_helpers.c export.c \
		libft.c linked_list.c pwd.c unset.c utils.c redirections.c helpers.c helpers1.c
INC = minishell.h
NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror -fsanitize=address
OBJS = $(addprefix obj/, $(SRCS:.c=.o))

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${FLAGS} ${OBJS} -lreadline -o ${NAME}

obj/%.o: %.c ${INC}
	@mkdir -p $(dir $@)
	${CC} ${FLAGS} -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -rf ${NAME}

re: fclean all

.PHONY: clean
