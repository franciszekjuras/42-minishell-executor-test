/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjuras <fjuras@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:28:02 by fjuras            #+#    #+#             */
/*   Updated: 2022/11/06 20:45:52 by fjuras           ###   ########.fr       */
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
#include <interface/env.h>
#include <interface/line.h>
#include <interface/test_framework.h>
#include <stdio.h>

#define GREP "/usr/bin/grep"
#define CAT "/usr/bin/cat"
#define MEG "./megaphone"

typedef struct s_test_data
{
	int		i;
	int		retval;
	int		retval_match;
	int		file_match;
}	t_test_data;

t_env	g_env;

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
	d.retval = minish_execute(&g_env, line);
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
	d.retval = minish_execute(&g_env, line);
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
	d.retval = minish_execute(&g_env, line);
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
	d.retval = minish_execute(&g_env, line);
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
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "THE", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_file_error_in_first(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], MEG, NULL);
	test_prog_redirs(&line.progs[d.i++], "./no/such/infile", "./no/such/outfile");
	test_prog_args(&line.progs[d.i], MEG, "hello", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "HELLO", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_file_error_in_last(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], MEG, "hello", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, "out/hello.txt");
	test_prog_args(&line.progs[d.i], GREP, "dog", NULL);
	test_prog_redirs(&line.progs[d.i++], "in/animals.txt", "no/such/outfile");
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/hello.txt", "HELLO", NULL);
	d.retval_match = test_expect_retval(d.retval, ENOENT);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_exe_error_in_first(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "./no/such/exe", NULL);
	test_prog_redirs(&line.progs[d.i++], "in/animals.txt", NULL);
	test_prog_args(&line.progs[d.i], MEG, "hello", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "HELLO", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_exe_error_in_last(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], MEG, "hello", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, "out/hello.txt");
	test_prog_args(&line.progs[d.i], "./no/such/exe", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, "out/out.txt");
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/hello.txt", "HELLO", NULL)
		& test_expect_file_size("out/out.txt", 0);
	d.retval_match = test_expect_retval(d.retval, ENOENT);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_dev_random_head(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], CAT, NULL);
	test_prog_redirs(&line.progs[d.i++], "/dev/random", NULL);
	test_prog_args(&line.progs[d.i], "/usr/bin/hexdump", "-n", "16" ,"-e", "/1 \"%02X\"", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_size("out/stdout.txt", 32);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_retval(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "/usr/bin/sleep", "0.1", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], GREP, NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = 1;
	d.retval_match = test_expect_retval(d.retval, ENOENT);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_echo(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], "echo", "yuhu", "hello", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "yuhu hello", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}


int	test_builtin_echo_empty(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], "echo", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_echo_n(const char *filter)
{
	t_line		line;
	t_test_data	d;

	TEST_START_CLEAN(filter);
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], "echo", "-n", "yuhu", "hello", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&g_env, line);
	printf("\n" TEST_NONL);
	fflush(stdout);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "yuhu hello", TEST_NONL, NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_2C_env_path(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("PATH=/rubbish:/usr/bin:/also/rubbish", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], MEG, "i", "am", "the", "walrus", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], "grep", "THE", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	minish_env_free(env);
	test_close_stdout();
	d.file_match = test_expect_file_content("out/stdout.txt", "THE", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_env(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("PATH=/usr/bin", "SFAG=yolo", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], "env", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, "out/out.txt");
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_content("out/out.txt", "PATH=/usr/bin", "SFAG=yolo", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_env_with_arg(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("PATH=/usr/bin", "SFAG=yolo", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 1);
	test_prog_args(&line.progs[d.i], "env", "some", "option", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, "out/out.txt");
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_size("out/out.txt", 0);
	d.retval_match = test_expect_retval(d.retval, EINVAL);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_export_new(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("AB=42", "DE=ye", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "export", "FG=lol", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], "env", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_content("out/stdout.txt", "AB=42", "DE=ye", "FG=lol", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_export_existing(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("AB=42", "DE=ye", "FG=lol", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "export", "DE=nut", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], "env", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_content("out/stdout.txt", "AB=42", "DE=nut", "FG=lol", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_export_invalid(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("AB=42", "DE=ye", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "export", "DE", "FG", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], "env", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_content("out/stdout.txt", "AB=42", "DE=ye", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_export_many(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("AB=42", "DE=ye", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "export", "FG=lol", "DE=xd", "FG=gee", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], "env", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_content("out/stdout.txt", "AB=42", "DE=xd", "FG=gee", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_export_path(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("AB=42", "PATH=/rubbish", "FG=lol", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "export", "PATH=/usr/bin", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], "grep", "dog", NULL);
	test_prog_redirs(&line.progs[d.i++], "in/animals.txt", NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_content("out/stdout.txt", "dog", NULL);
	d.retval_match = test_expect_retval(d.retval, 0);
	return (TEST_END(d.retval_match && d.file_match));
}

int	test_builtin_unset(const char *filter)
{
	t_line		line;
	t_test_data	d;
	t_env		env;
	char		**environ;

	TEST_START_CLEAN(filter);
	environ = test_make_environ("AB=42", "DE=ye", "FG=lol", "HI=hello", NULL);
	minish_env_init(&env, environ);
	test_free_environ(environ);
	d.i = 0;
	test_line_init(&line, 2);
	test_prog_args(&line.progs[d.i], "unset", "DE", "NONE", "HI", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_prog_args(&line.progs[d.i], "env", NULL);
	test_prog_redirs(&line.progs[d.i++], NULL, NULL);
	test_line_end(&line, d.i);
	test_redirect_stdout("out/stdout.txt");
	d.retval = minish_execute(&env, line);
	test_close_stdout();
	minish_env_free(env);
	d.file_match = test_expect_file_content("out/stdout.txt", "AB=42", "FG=lol", NULL);
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
	test_2C_file_error_in_first,
	test_2C_file_error_in_last,
	test_2C_exe_error_in_first,
	test_2C_exe_error_in_last,
	test_2C_dev_random_head,
	test_2C_retval,
	test_builtin_echo,
	test_builtin_echo_empty,
	test_builtin_echo_n,
	test_2C_env_path,
	test_builtin_env,
	test_builtin_env_with_arg,
	test_builtin_export_new,
	test_builtin_export_existing,
	// test_builtin_export_invalid,
	test_builtin_export_path,
	test_builtin_unset,
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
