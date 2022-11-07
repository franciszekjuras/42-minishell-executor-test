NAME = test
NAME2 = test_sysfail

FILES = \
	test

FILES2 = \
	test_sysfail

HEADERS = \
	../interface/line ../interface/test_framework

HFILES += $(HEADERS:%=%.h)
OFILES = $(FILES:%=%.o)
OFILES2 = $(FILES2:%=%.o)
CFLAGS = -Wall -Wextra -Werror
OPTIM = -ggdb3
export OPTIM
INC = -I..
LIBS= -lexecutor -lft -lgcov --coverage
LIBS_FILES_ = executor/libexecutor.a libft/libft.a
LIBS_FILES = $(addprefix ../, $(LIBS_FILES_))
LIBS_DIRS = $(addprefix -L, $(dir $(LIBS_FILES)))

all: $(NAME) $(NAME2) megaphone

megaphone: megaphone.c
	gcc $(CFLAGS) megaphone.c -o megaphone

$(NAME): $(OFILES) $(LIBS_FILES)
	gcc $(LIBS_DIRS) $(OFILES) $(LIBS) -o $@

$(NAME2): $(OFILES2) $(LIBS_FILES)
	gcc $(LIBS_DIRS) $(OFILES2) $(LIBS) -o $@

$(OFILES): %.o: %.c $(HFILES)
	gcc $(CFLAGS) $(OPTIM) $(INC) -c $< -o $@

$(OFILES2): %.o: %.c $(HFILES)
	gcc $(CFLAGS) $(OPTIM) $(INC) -c $< -o $@

$(LIBS_FILES): FORCE
	make -C ../libft
	make -C ../executor

FORCE: ;

clean_:
	rm -f $(OFILES) $(OFILES2)

clean: clean_libs clean_

fclean_: clean_
	rm -f $(NAME)
	rm -f $(NAME2)
	rm -f megaphone

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

libs_cov_:
	make -C ../libft
	make re -C ../executor COV_FLAGS="-fprofile-arcs -ftest-coverage"

cov: libs_cov_ all
	./run.sh
	make -C ../executor report
	make -C ../executor fclean

.PHONY: all clean clean_ fclean fclean_ re re_ re_libs clean_libs FORCE
