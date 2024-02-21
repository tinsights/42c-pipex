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
#include "libft.h"


int	main(int ac, char **av, char ** envp)
{
	if (ac != 3)
		return printf("Usage: ./pipex file1 cmd1\n");


	char **PATHS;

	char *file1;
	// char *file2;
	char **cmd1;
	// char *cmd2;
	int 	i;

	file1 = av[1];
	cmd1 = ft_split(av[2], ' ');
	printf("< %s %s\n", file1, cmd1[0]);
	// printf("cat %s | %s\n", file1, cmd1[0]);

	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH", 4))
		{
			PATHS = ft_split(*envp + 5, ':');
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

	char *binaryPath = PATHS[j];
  	execve(binaryPath, cmd1, envp);
  	perror("");


		i = 0;
	while (PATHS[i])
		free(PATHS[i++]);
	free (PATHS);
	
	i = 0;
	while (cmd1[i])
		free(cmd1[i++]);
	free(cmd1);
}
