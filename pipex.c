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
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"


int	main(int ac, char **av, char ** envp)
{
	if (ac != 5)
		return printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");

	char **PATHS;

	char *file1;
	char *file2;
	char **cmd1;
	char **cmd2;
	int 	i;

	file1 = av[1];
	file2 = av[4];
	cmd1 = ft_split(av[2], ' ');
	cmd2 = ft_split(av[3], ' ');


	char** PATHS2;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH", 4))
		{
			PATHS = ft_split(*envp + 5, ':');
			PATHS2 = ft_split(*envp + 5, ':');
		}
		envp++;
	}


	int j = 0;
	while (PATHS[j])
	{
		char *cmd = ft_strjoin("/", cmd1[0]);
		char *binpath = ft_strjoin(PATHS[j], cmd);
		free(cmd);
		free(PATHS[j]);
		PATHS[j] = binpath;
		if (!access(PATHS[j], X_OK))
			break;
		j++;
	}
	if (PATHS[j] == NULL)
		printf("%s: command not found\n", cmd1[0]);


	int p_fd[2];
	pipe(p_fd); // error checking later

	int pid = fork();
	
	if (pid == 0)
	{
		close(p_fd[0]); // we are never reading
		int fd = open(file1, O_RDWR | O_CREAT, 0777);
		dup2(fd, STDIN_FILENO);
		close(fd);
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
		char *binaryPath = PATHS[j];
		execve(binaryPath, cmd1, envp);
	}
	else
	{	
		close(p_fd[1]); // we are never writing

		int j = 0;
		while (PATHS2[j])
		{
			char *cmd = ft_strjoin("/", cmd2[0]);
			char *binpath = ft_strjoin(PATHS2[j], cmd);
			free(cmd);
			free(PATHS2[j]);

			PATHS2[j] = binpath;
			if (!access(PATHS2[j], X_OK))
				break;
			j++;
		}
		if (PATHS2[j] == NULL)
			printf("%s: command not found\n", cmd2[0]);

		dup2(p_fd[0], STDIN_FILENO);
		if (!access(file2, X_OK))
			unlink(file2);
		int fd = open(file2, O_WRONLY | O_CREAT, 0777);
		dup2(fd, STDOUT_FILENO);
		char *binaryPath = PATHS2[j];
		execve(binaryPath, cmd2, envp);

	}


	i = 0;
	while (PATHS[i])
		free(PATHS[i++]);
	free (PATHS);
	
	i = 0;
	while (cmd1[i])
		free(cmd1[i++]);
	free(cmd1);
}
