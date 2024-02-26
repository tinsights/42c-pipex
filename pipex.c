/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:23:46 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/21 13:32:10 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
// #include "libft.h"


int	main(int ac, char **av)
{
	if (ac != 3)
		return printf("Usage: ./pipex file1 cmd1\n");


	// char **PATHS;

	char *file1;
	char *cmd1;
	int		p_fd[2];
	int		p2_fd[2];
	time_t 	t;

	file1 = av[1];
	cmd1 = av[2];

	srand((unsigned) time(&t));
	if (pipe(p_fd) < 0)
		return (-1);
	if (pipe(p2_fd) < 0)
		return (-1);

	int pid = fork();
	if (pid  == -1)
		return (-2);

	if (pid == 0)
	{
		// child process
		int i;
		int recvd;

		i = rand() % 42;
		printf("child generated %i\n", i);

		write(p_fd[1], &i, 4);
		read(p_fd[0], &recvd, 4);
		printf("child received %i\n", recvd);

	}
	else
	{
		// parent process
		int i;
		int recvd;

		read(p_fd[0], &recvd, 4);
		printf("parent received %i\n", recvd);

		i = rand() % recvd;
		printf("parent generated %i\n", i);

		write(p2_fd[1], &i, 4);

		
	}
	close(p_fd[1]);
	close(p_fd[0]);

}
