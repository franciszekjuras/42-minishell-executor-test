/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjuras <fjuras@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:28:02 by fjuras            #+#    #+#             */
/*   Updated: 2022/10/28 15:52:38 by fjuras           ###   ########.fr       */
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


int	test_single_cmd_no_args(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
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

int	test_single_cmd_one_arg(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
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

int	test_1C_in_and_out_redir(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
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

int	test_2C_2oR_1iR(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
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

int	test_2C_pipe(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], MEG, "i", "am", "the", "walrus", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], GREP, "THE", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "THE", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

const t_test_function g_test_functions[] =
{
	test_single_cmd_no_args,
	test_single_cmd_one_arg,
	test_1C_in_and_out_redir,
	test_2C_2oR_1iR,
	test_2C_pipe,
	NULL
};

int main (int argc, char **argv)
{
	return (test_main(argc, argv));
}
