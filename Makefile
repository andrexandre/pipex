END		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
CYAN 	= \033[1;36m

NAME	=	pipex

SRC		= $(wildcard *.c)
OBJ		= $(SRC:.c=.o)
RM		=	rm -f

all:	$(NAME)

$(NAME):	$(OBJ)
	@cc $(OBJ) -o $(NAME)
	@mv *.o objs
	@echo "$(GREEN)\nStuff compiled 🛠️\n$(END)"

%.o : %.c
	@mkdir -p objs
	@cc -Wall -Wextra -Werror -c $< -o $@

clean:
	@$(RM) objs/$(OBJ)
	@echo "Stuff removed 🗑️"

fclean:	clean
	@$(RM) $(NAME)
	@$(RM)r objs
	@echo "$(GREEN)\nAll stuff removed 🗑️\n$(END)"

re:	fclean all

run: ${NAME}
	@./${NAME}

t	:= 1

test: ${NAME}
	./${NAME} infile cat "wc -c" outfile
	@sleep ${t}
	./${NAME} infile cat wc outfile
	@sleep ${t}
	./${NAME} infile notcat wc outfile
	@sleep ${t}
	./${NAME} infile cat notwc outfile

testt:
	< infile cat | wc -c > outfile
	@sleep ${t}
	< infile cat | wc > outfile
	@sleep ${t}
	< infile notcat | wc > outfile
	@sleep ${t}
	< infile cat | notwc > outfile

# make it so the errors are the same :)
# val:	${NAME}
# 	@valgrind ./${NAME}

e: fclean

.PHONY:	all clean fclean re