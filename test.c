/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjuras <fjuras@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:28:02 by fjuras            #+#    #+#             */
/*   Updated: 2022/10/27 23:37:30 by fjuras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Test name abbreviations:
#C -- # commands
iR -- input redirection
oR -- output redirection
*/

#include <stdlib.h>
#include <string.h>
#include <executor/executor.h>
#include <interface/line.h>
#include <interface/test_line.h>
#include <stdio.h>

#define GREP "/usr/bin/grep"
#define MEG "./megaphone"

typedef struct s_test_data
{
	int		i;
	int		retval;
	int		retval_match;
	int		file_match;
}	t_test_data;


int	test_single_cmd_no_args(void)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN();
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], MEG, NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "NOISE", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_single_cmd_one_arg(void)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN();
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], MEG, "hello", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "HELLO", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_1C_in_and_out_redir(void)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN();
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], GREP, "dog", NULL);
	test_prog_redirs(&line.progs[d.i++], "in/animals.txt", "out/out.txt");
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", NULL)
		& test_expect_file_content("out/out.txt", "dog", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_2oR_1iR(void)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN();
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], MEG, "one", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, "out/out1.txt");
	test_prog_args(&line.progs[d.i], MEG, "two", NULL);
	test_prog_redirs(&line.progs[d.i++], "in/animals.txt", "out/out2.txt");
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", NULL)
		& test_expect_file_content("out/out1.txt", "ONE", NULL)
		& test_expect_file_content("out/out2.txt", "TWO", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int (*const	g_test_functions[])() = {
	test_single_cmd_no_args,
	test_single_cmd_one_arg,
	test_1C_in_and_out_redir,
	test_2C_2oR_1iR,
	NULL
};

int	main(void)
{
	int	passed;
	int	total;

	passed = 0;
	total = 0;
	while (g_test_functions[total] != NULL)
	{
		passed += g_test_functions[total]();
		++total;
	}
	fprintf(stderr, "^^^\n");
	if (passed == total)
		fprintf(stderr, "    %s all %d tests passed\n", TEST_STR_OK, total);
	else
		fprintf(stderr, "    %s %d of %d tests failed\n", TEST_STR_FAIL,
			total - passed, total);
	fprintf(stderr, "^^^\n");
	close(0);
	close(2);
	return (passed < total);
}
