
#include <stdlib.h>
#include <string.h>
#include <executor/executor.h>
#include <interface/line.h>
#include <interface/test_line.h>
#include <stdio.h>

int	test_simplest(void)
{
	t_line	line;
	int		i;
	int		retval;
	int		res;

	TEST_START();
	i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[i], 1, "ls");
	test_prog_redirs(&line.progs[i++], NULL, NULL);
	test_line_end(&line, i);
	retval = minish_execute(line);
	res = test_expect_retval(retval, 0);
	return (TEST_END(res));
}

int (*const test_functions[])() = {
	test_simplest,
    NULL
};

int	main(void)
{
	int	passed;
	int	total;

	passed = 0;
	total = 0;
	while (test_functions[total] != NULL)
	{
		passed += test_functions[total]();
		++total;
	}
	fprintf(stderr, "^^^\n");
	if (passed == total)
		fprintf(stderr, "    %s all %d tests passed\n", TEST_STR_OK, total);
	else
		fprintf(stderr, "    %s %d of %d tests failed\n", TEST_STR_FAIL,
			total - passed, total);
	fprintf(stderr, "^^^\n");
	return (passed < total);
}
