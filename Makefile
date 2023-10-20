END		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
CYAN 	= \033[1;36m

RM		= rm -f
NAME	= pipex

CFLAGS = -Wall -Wextra -Werror -g

SRCDIR	= srcs
OBJDIR	= objs

# wildcard illegal
# SRC		= ft_split.c  pipex_utils.c  tool_lib2.c  tool_lib3.c  tool_lib.c $(NAME).c
SRC		= $(wildcard $(SRCDIR)/*.c) $(NAME).c
OBJ		= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

# BSRC	= ft_split.c  pipex_utils.c  tool_lib2.c  tool_lib3.c  tool_lib.c $(NAME)_bonus.c
BSRC	= $(wildcard $(SRCDIR)/*.c) $(NAME)_bonus.c
BOBJ	= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(BSRC))

all:	$(NAME)

# $(NAME): $(OBJ) # normal
$(NAME): $(BOBJ) # bonus
	@cc $(CFLAGS) $^ -o $@
	@echo "$(GREEN)\nStuff compiled 🛠️\n$(END)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@cc $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

# BNAME	= pipex_bonus

# bonus: $(BNAME)

# $(BNAME): $(BOBJ)
# 	@cc $(CFLAGS) $^ -o $@
# 	@echo "$(GREEN)\nBonus compiled 🛠️\n$(END)"

# bonus: $(BOBJ) $(NAME)
# 	@cc $(CFLAGS) $(BOBJ) -o $(NAME)
# 	@echo "$(GREEN)\nBonus compiled 🛠️\n$(END)"

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

TESTF	= infile cat wc outfile

v:
	@make && valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all ./${NAME} ${TESTF}

val:	${NAME}
	@output=$$(make re && valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all ./${NAME} ${TESTF} 2>&1); \
	if echo "$$output" | grep -q 'freed' && echo "$$output" | grep -q '0 errors' ; then\
		echo -n "$(GREEN)"; echo "$$output" | grep -E 'freed|total|ERROR S|file descriptor' | sed 's/^[^ ]* //';\
	else\
		echo -n "$(RED)"; echo "$$output" | grep -E 'total|ERROR S|file descriptor' | sed 's/^[^ ]* //';\
	fi;

e: fclean

.PHONY:	all clean fclean re run test testt v val e