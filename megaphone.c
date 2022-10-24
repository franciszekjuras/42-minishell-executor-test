/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjuras <fjuras@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:28:29 by fjuras            #+#    #+#             */
/*   Updated: 2022/10/24 18:28:37 by fjuras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <ctype.h>

int	main(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc == 1)
	{
		printf("NOISE\n");
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
			printf("%c", toupper(argv[i][j++]));
		printf("\n");
		++i;
	}
	return (0);
}
