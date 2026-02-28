SRCS = Minishell.c parsing.c split.c string_lib.c error.c quotes.c cmd_list.c \
		expand.c env_list.c export.c builtins.c builtins_impl.c execution.c \
		utils.c redirections.c heredoc.c signals.c
INC = minishell.h
NAME = minishell
CC = cc
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)
	READLINE_INCLUDE := $(READLINE_PREFIX)/include
	READLINE_LIB := $(READLINE_PREFIX)/lib
	INCLUDES := -I$(READLINE_INCLUDE)
	LIBS := -L$(READLINE_LIB) -lreadline
else
	INCLUDES :=
	LIBS := -lreadline
endif
FLAGS = -Wall -Wextra -Werror
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
