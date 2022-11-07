/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_sysfail.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjuras <fjuras@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:28:02 by fjuras            #+#    #+#             */
/*   Updated: 2022/11/07 14:19:22 by fjuras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Test name abbreviations:
#C -- # commands
iR -- input redirection
oR -- output redirection
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <executor/executor.h>
#include <interface/env.h>
#include <interface/line.h>
#include <interface/test_framework.h>

#define GREP "/usr/bin/grep"
#define CAT "/usr/bin/cat"
#define MEG "./megaphone"
#define SEGF "./segfaulter.sh"

#define SIGCODE(x) (128 + (x))

typedef struct s_test_data
{
	int		i;
	int		retval;
	int		retval_match;
	int		file_match;
}	t_test_data;

t_env	g_env;

int	test_2C_2nd_fork_fail(const char *filter)
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
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", NULL);
	d.retval_match = test_expect_retval(d.retval, EAGAIN);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_pipe_fail(const char *filter)
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
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", NULL);
	d.retval_match = test_expect_retval(d.retval, ENFILE);
	return (TEST_END(d.retval_match && d.file_match));
}

const t_test_function g_test_functions[] =
{
	test_2C_2nd_fork_fail,
	test_2C_pipe_fail,
	NULL
};

int main (int argc, char **argv)
{
	int	retval;

	minish_env_init(&g_env, NULL);
	retval = test_main(argc, argv);
	minish_env_free(g_env);
	return (retval);
}
