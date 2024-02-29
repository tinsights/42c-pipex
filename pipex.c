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

#include "pipex.h"

typedef struct s_params
{
	int		num_cmds;
	char	**paths;
	char	***cmds;
	int		fd[3];

}	t_params;

char *check_valid_cmd(char **paths, char *cmd)
{
	int		j;
	char	*cmdpath;
	char	*binpath;

	cmdpath = ft_strjoin("/", cmd);
	j = 0;
	while (paths && paths[j])
	{
		binpath = ft_strjoin(paths[j], cmdpath);
		if (!access(binpath, X_OK))
			break ;
		free(binpath);
		j++;
	}
	free(cmdpath);
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
		free(binpath);
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

int	init_data(int ac, char **av, t_params *p, char **envp)
{
	int	i;

	p->fd[0] = open(av[1], O_RDONLY);
	if (p->fd[0] < 0)
	{
		perror(av[1]);
		p->fd[0] = open("/dev/null", O_RDONLY);
		ac--;
		av++;
	}
	p->fd[1] = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	p->num_cmds = ac - 3;
	p->cmds = (char ***) ft_calloc(p->num_cmds + 1, sizeof(char **));
	i = -1;
	while (++i < p->num_cmds)
		separate_quotes(av[i + 2], &(p->cmds[i]));
	while (*envp && ft_strncmp(*envp, "PATH", 4))
		envp++;
	if (*envp)
		p->paths = ft_split(*envp + 5, ':');
	else
		p->paths = (char**) (ft_calloc(1, sizeof(char *)));
	if (p->fd[1] < 0)
	{
		perror(av[ac - 1]);
		return (0);
	}
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char		c;
	t_params	params;

	if ((!BONUS && ac != 5) || (BONUS && ac < 5))
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		return (-1);
	}
	if (init_data(ac, av, &params, envp))
	{
		dup2(params.fd[0], STDIN_FILENO);
		close(params.fd[0]);
		recurse_pipe(params.paths, params.cmds);
		while (read(STDIN_FILENO, &c, 1))
			write(params.fd[1], &c, 1);
	}
	free_memory(params.cmds, params.paths);
}
