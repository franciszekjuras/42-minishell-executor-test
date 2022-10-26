/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjuras <fjuras@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:28:02 by fjuras            #+#    #+#             */
/*   Updated: 2022/10/26 22:16:18 by fjuras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#C -- # commands
*/

#include <stdlib.h>
#include <string.h>
#include <executor/executor.h>
#include <interface/line.h>
#include <interface/test_line.h>
#include <stdio.h>

#define GREP "/usr/bin/grep"

int	test_single_cmd_no_args(void)
{
	t_line	line;
	int		i;
	int		retval;
	int		res;
	int		stdout_fd;
	int		file_match;

	TEST_START_CLEAN();
	i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[i], "./megaphone", NULL);
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

	TEST_START_CLEAN();
	i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[i], "./megaphone", "hello", NULL);
	test_prog_redirs(&line.progs[i++], NULL, NULL);
	test_line_end(&line, i);
	stdout_fd = test_redirect_stdout("out/stdout.txt");
	retval = minish_execute(line);
	test_restore_stdout(stdout_fd);
	file_match = test_expect_file_content("out/stdout.txt", "HELLO", NULL);
	res = test_expect_retval(retval, 0);
	return (TEST_END(res && file_match));
}

int	test_1C_in_and_out_redir(void)
{
	t_line	line;
	int		i;
	int		retval;
	int		res;
	int		stdout_fd;
	int		file_match;

	TEST_START_CLEAN();
	i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[i], GREP, "dog", NULL);
	test_prog_redirs(&line.progs[i++], "in/animals.txt", "out/out.txt");
	test_line_end(&line, i);
	stdout_fd = test_redirect_stdout("out/stdout.txt");
	retval = minish_execute(line);
	test_restore_stdout(stdout_fd);
	file_match = test_expect_file_content("out/stdout.txt", NULL)
		& test_expect_file_content("out/out.txt", "dog", NULL);
	res = test_expect_retval(retval, 0);
	return (TEST_END(res && file_match));
}

int (*const	g_test_functions[])() = {
	test_single_cmd_no_args,
	test_single_cmd_one_arg,
	test_1C_in_and_out_redir,
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
	return (passed < total);
}
