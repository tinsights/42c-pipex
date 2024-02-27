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

#ifndef BONUS
# define BONUS 0
#endif

typedef struct s_params
{
	int		num_cmds;
	char	**paths;
	char	***cmds;
}	t_params;

char	*check_valid_cmd(char **paths, char *cmd)
{
	int		j;
	char	*binpath;
	char	*cmdpath;

	j = 0;
	while (paths[j])
	{
		cmdpath = ft_strjoin("/", cmd);
		binpath = ft_strjoin(paths[j], cmdpath);
		free(cmdpath);
		if (!access(binpath, X_OK))
			break ;
		free(binpath);
		j++;
	}
	if (paths[j] == NULL)
		return (NULL);
	return (binpath);
}

void	run_child_command(int p_fd[2], char **paths, char **cmd)
{
	char	*binpath;

	close(p_fd[0]);
	binpath = check_valid_cmd(paths, cmd[0]);
	if (!binpath)
	{
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else
	{
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
		execve(binpath, cmd, NULL);
		perror("");
	}
}

void	recurse_pipe(char **paths, char ***cmds)
{
	int		p_fd[2];
	int		pid;

	pipe(p_fd);
	pid = fork();
	if (pid == 0)
		run_child_command(p_fd, paths, cmds[0]);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		wait(NULL);
		if (*(++cmds))
			recurse_pipe(paths, cmds);
		else
			return ;
	}
}

void	free_memory(char ***cmds, char **paths)
{
	int		i;
	int		j;
	char	**cmd;

	i = 0;
	while (cmds[i])
	{
		cmd = cmds[i];
		j = -1;
		while (cmd[++j])
			free(cmd[j]);
		free(cmd);
		i++;
	}
	free(cmds);
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
}

int	init_data(int ac, char **av, t_params *params, char **envp)
{
	int	i;

	params->num_cmds = ac - 3;
	params->cmds = ft_calloc(params->num_cmds + 1, sizeof(char *));
	params->cmds[params->num_cmds] = NULL;
	i = -1;
	while (++i < params->num_cmds)
		params->cmds[i] = ft_split(av[i + 2], ' ');
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH", 4))
		{
			params->paths = ft_split(*envp + 5, ':');
			break ;
		}
		envp++;
	}
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char		c;
	int			fd[2];
	t_params	params;

	if ((!BONUS && ac != 5) || (BONUS && ac < 5))
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		return (-1);
	}
	init_data(ac, av, &params, envp);
	fd[0] = open(av[1], O_RDONLY, 0777);
	fd[1] = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd[0] < 0)
		perror(av[1]);
	if (fd[1] < 0)
		perror(av[ac - 1]);
	if (fd[0] >= 0 && fd[1] >= 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		recurse_pipe(params.paths, params.cmds);
		while (read(STDIN_FILENO, &c, 1))
			write(fd[1], &c, 1);
	}
	free_memory(params.cmds, params.paths);
}
