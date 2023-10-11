END		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
CYAN 	= \033[1;36m

SRCDIR  = .
OBJDIR  = objs
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

RM		= rm -f
NAME	= pipex

all:	$(NAME)

$(NAME): $(OBJ)
	@cc $^ -o $@
	@echo "$(GREEN)\nStuff compiled 🛠️\n$(END)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@cc -Wall -Wextra -Werror -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJDIR)/*.o
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

.PHONY:	all clean fclean re run test testt