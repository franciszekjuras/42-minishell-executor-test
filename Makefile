NAME = test

FILES = \
	test

HEADERS = \
	../interface/line

HFILES += $(HEADERS:%=%.h)
OFILES = $(FILES:%=%.o)
CFLAGS = -Wall -Wextra -Werror
OPTIM = -ggdb3
export OPTIM
INC = -I.
LIBS= -lft -lexecutor
LIBS_FILES_ = libft/libft.a executor/libexecutor.a
LIBS_FILES = $(addprefix ../, $(LIBS_FILES_))
LIBS_DIRS = $(addprefix -L, $(dir $(LIBS_FILES)))

all: $(NAME)

$(NAME): $(OFILES) $(LIBS_FILES)
	gcc $(LIBS_DIRS) $(OFILES) $(LIBS) -o $@

$(OFILES): %.o: %.c $(HFILES)
	gcc $(CFLAGS) $(OPTIM) $(INC) -c $< -o $@

$(LIBS_FILES): FORCE
	make -C $(dir $@)

FORCE: ;

clean_:
	rm -f $(OFILES)

clean: clean_libs clean_

fclean_: clean_
	rm -f $(NAME)

fclean: fclean_ clean
	rm -f $(LIBS_FILES)

re_: fclean_ all

re: re_libs re_

clean_libs:
	make clean -C ../libft
	make clean -C ../executor

re_libs:
	make re -C ../libft
	make re -C ../executor

.PHONY: all clean clean_ fclean fclean_ re re_ re_libs clean_libs FORCE
