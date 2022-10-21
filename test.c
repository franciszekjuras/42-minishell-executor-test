
#include <stdlib.h>
#include <string.h>
#include <executor/executor.h>
#include <interface/line.h>
#include <interface/test_line.h>
#include <stdio.h>

int	test_single_cmd_no_args(void)
{
	t_line	line;
	int		i;
	int		retval;
	int		res;
	int		stdout_fd;
	int		file_match;

	TEST_START();
	i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[i], 1, "./megaphone");
	test_prog_redirs(&line.progs[i++], NULL, NULL);
	test_line_end(&line, i);
	stdout_fd = test_redirect_stdout("out/stdout.txt");
	retval = minish_execute(line);
	test_restore_stdout(stdout_fd);
	file_match = test_expect_file_content("out/stdout.txt", "NOISE", NULL);
	res = test_expect_retval(retval, 0);
	return (TEST_END(res && file_match));
}

int	test_single_cmd_one_arg(void)
{
	t_line	line;
	int		i;
	int		retval;
	int		res;
	int		stdout_fd;
	int		file_match;

	TEST_START();
	i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[i], 2, "./megaphone", "hello");
	test_prog_redirs(&line.progs[i++], NULL, NULL);
	test_line_end(&line, i);
	stdout_fd = test_redirect_stdout("out/stdout.txt");
	retval = minish_execute(line);
	test_restore_stdout(stdout_fd);
	file_match = test_expect_file_content("out/stdout.txt", "HELLO", NULL);
	res = test_expect_retval(retval, 0);
	return (TEST_END(res && file_match));
}

int (*const test_functions[])() = {
	test_single_cmd_no_args,
	test_single_cmd_one_arg,
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
