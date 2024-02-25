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
// #include "libft.h"


int	main(int ac, char **av)
{
	if (ac != 3)
		return printf("Usage: ./pipex file1 cmd1\n");


	// char **PATHS;

	char *file1;
	char *cmd1;
	int		p_fd[2];

	file1 = av[1];
	cmd1 = av[2];

	if (pipe(p_fd) < 0)
		return (-1);

	// int fd = open("file2", O_RDWR | O_CREAT, 0777);
	// dup2(fd, p_fd[0]);

	int pid = fork();
	if (pid  == -1)
		return (-2);

	if (pid == 0)
	{
		// child process
		//p_fd[0] read
		//p_fd[1] write
		write(1, "child\n", 6);

		// child process
		int fd = open(file1, O_RDONLY | O_CREAT);
		dup2(fd, STDIN_FILENO);
		close(fd);

		close(p_fd[0]); // we are never reading
		dup2(p_fd[1], STDOUT_FILENO);
		sleep(3);
		execlp("tr" ,"tr", "-d", "l", NULL);
		close(p_fd[1]);
	}
	else
	{
		// parent process
		close(p_fd[1]); // we are never writing
		write(1, "parent\n", 8);

		// dup2(p_fd[0], STDIN_FILENO);
		// close(p_fd[0]);
		char c;
		int nb = read(p_fd[0], &c, 1);
		write(1, &nb, 4);
		while (nb > 0)
		{
			write(1, &c, 1);
			nb = read(p_fd[0], &c, 1);

		}
		wait(NULL);
		
	}
	close(p_fd[1]);
	close(p_fd[0]);

}
