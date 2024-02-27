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
#include <sys/wait.h>
#include "libft.h"


char *check_valid_cmd(char **PATHS, char *cmd)
{
	int j = 0;
	char *binpath;
	while (PATHS[j])
	{
		char *cmdpath = ft_strjoin("/", cmd);
		binpath = ft_strjoin(PATHS[j], cmdpath);
		free(cmdpath);
		if (!access(binpath, X_OK))
			break;
		free(binpath);
		j++;
	}
	if (PATHS[j] == NULL)
		return NULL;
	return binpath;
}

/*
TODOS:
1) Base case for recursion: if last command reached
2) create array of commands in main
3) write to file prior to base case
i.e. if next cmd is last cmd, redirect stdout to file.
*/
void run_child_process(char **PATHS, char ***cmds, char **envp)
{
	int p_fd[2];
	pipe(p_fd); // error checking later
	char *binpath;
	char **cmd = cmds[0];
	int pid = fork();

	if (pid == 0)
	{	// child
		close(p_fd[0]); // we are never reading

		//check the command is valid
		binpath = check_valid_cmd(PATHS, cmd[0]);
		if (!binpath)
			printf("%s: command not found\n", cmd[0]);

		// output goes to pipe instead of stdout
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);

		//execute
		execve(binpath, cmd, envp);
		perror("");
	}
	else
	{	// PARENT PROCESS

		close(p_fd[1]); // we are never writing

		// binpath = check_valid_cmd(PATHS, cmd2[0]);
		// if (!binpath)
		// 	printf("%s: command not found\n", cmd2[0]);

		// int fd = open(file2, O_WRONLY | O_TRUNC | O_CREAT, 0777);



		// dup2(fd, STDOUT_FILENO);
		// close(fd);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		wait(NULL);
		if (*(++cmds))
		{

			run_child_process(PATHS, cmds, envp);
		}
		else
		{
			return ;
		}
	}

}


int	main(int ac, char **av, char ** envp)
{
	if (ac != 5)
		return printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");

	char **PATHS;

	char *file1;
	char *file2;
	char ***cmds;

	file1 = av[1];
	file2 = av[4];

	int num_of_cmds = ac - 3;
	cmds = ft_calloc(num_of_cmds + 1, sizeof(char *));
	cmds[num_of_cmds] = NULL;

	int i = 0;
	while (i < num_of_cmds)
	{
		char **cmd = ft_split(av[i + 2], ' ');
		cmds[i] = cmd;
		i++;
	}

	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH", 4))
		{
			PATHS = ft_split(*envp + 5, ':');
		}
		envp++;
	}

	// open file1, replace with stdin
	int fd = open(file1, O_RDWR | O_CREAT, 0777);
	dup2(fd, STDIN_FILENO);
	close(fd);

	run_child_process(PATHS, cmds, envp);

	fd = open(file2, O_WRONLY | O_TRUNC | O_CREAT, 0777);

	char c;
	while(read(STDIN_FILENO, &c, 1))
		write(fd, &c, 1);



}
