END			:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m

NAME	=	pipex

RM	=	rm -f

SOURCES	=	pipex.c

OBJECTS	= $(SOURCES:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJECTS) $(INCLUDE)
	@cc $(OBJECTS) -o $(NAME)
	@echo "$(GREEN)\nStuff compiled 🛠️\n$(END)"

%.o : %.c
	@cc -Wall -Wextra -Werror -c $< -o $@

clean:
	@$(RM) $(OBJECTS)
	@echo "Stuff removed 🗑️"

fclean:	clean
	@$(RM) $(NAME)
	@echo "$(GREEN)\nAll stuff removed 🗑️\n$(END)"

re:	fclean all

run: ${NAME}
	@./${NAME}

val:	${NAME}
	@valgrind ./${NAME}

e: fclean

.PHONY:	all clean fclean re