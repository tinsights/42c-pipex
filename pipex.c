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
	char	*delim;
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
	if (cmd && cmd[0])
		binpath = check_valid_cmd(paths, cmd[0]);
	else 
		binpath = NULL;
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
		if (binpath)
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
		if (*cmds && *(++cmds))
			recurse_pipe(paths, cmds);
		waitpid(pid, NULL, WNOHANG);
	}
}

void	free_memory(char ***cmds, char **paths)
{
	int		i;
	int		j;
	char	**cmd;

	i = 0;
	while (cmds && cmds[i])
	{
		cmd = cmds[i];
		j = -1;
		while (cmd[++j])
			free(cmd[j]);
		free(cmd);
		i++;
	}
	if (cmds)
		free(cmds);
	i = -1;
	while (paths && paths[++i])
		free(paths[i]);
	if (paths)
		free(paths);
}


void open_files(int *ac, char ***av, t_params *p)
{
	if (BONUS && !ft_strncmp((*av)[1], "here_doc", 9))
	{
		int p_fd[2];

		p->fd[1] = open((*av)[(*ac) - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (p->fd[1] < 0)
		{
			perror((*av)[(*ac) - 1]);
			(*ac)--;
		}
		pipe(p_fd);
		p->fd[0] = p_fd[0];
		p->fd[2] = p_fd[1];
	}
	else
	{
		p->fd[1] = open((*av)[*ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (p->fd[1] < 0)
		{
			perror((*av)[*ac - 1]);
			(*ac)--;
		}
		p->fd[0] = open((*av)[1], O_RDONLY);
		if (p->fd[0] < 0)
		{
			perror((*av)[1]);
			p->fd[0] = open("/dev/null", O_RDONLY);
			(*ac)--;
			(*av)++;
		}	
	}
}

int	init_data(int ac, char **av, t_params *p, char **envp)
{
	int	i;

	open_files(&ac, &av, p);
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
	return (1);
}

int init_heredoc(int ac, char **av, t_params *p, char **envp)
{
	int	i;
	
	p->delim = av[2];
	open_files(&ac, &av, p);
	p->num_cmds = ac - 4;
	p->cmds = (char ***) ft_calloc(p->num_cmds + 1, sizeof(char **));
	i = -1;
	while (++i < p->num_cmds)
		separate_quotes(av[i + 3], &(p->cmds[i]));
	while (*envp && ft_strncmp(*envp, "PATH", 4))
		envp++;
	if (*envp)
		p->paths = ft_split(*envp + 5, ':');
	else
		p->paths = (char**) (ft_calloc(1, sizeof(char *)));
	return (1);
}

void run_here_doc(int ac, char **av, char **envp, t_params *p)
{
	if (init_heredoc(ac, av, p, envp))
	{
		int len = ft_strlen(p->delim);
		char  *line = get_next_line(STDIN_FILENO);
		while (line)
		{
			int i = ft_strncmp(line, p->delim, len);
			if (!i)
				break;
			write(p->fd[2], line, ft_strlen(line));
			free(line);
			line = get_next_line(STDIN_FILENO);
		}
		free(line);
		get_next_line(-1);
		close(p->fd[2]);
		dup2(p->fd[0], STDIN_FILENO);
		close(p->fd[0]);
		if (*(p->cmds) && *(p->paths))
			recurse_pipe(p->paths, p->cmds);	
	}
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
	if (BONUS && !ft_strncmp(av[1], "here_doc", 9))
		run_here_doc(ac, av, envp, &params);
	else
	{
		init_data(ac, av, &params, envp);
		dup2(params.fd[0], STDIN_FILENO);
		close(params.fd[0]);
		if (*(params.cmds) && *(params.paths))
			recurse_pipe(params.paths, params.cmds);
	}
	if (params.fd[1] > 0)
	{
		while (read(STDIN_FILENO, &c, 1))
			write(params.fd[1], &c, 1);
		close(params.fd[1]);
	}
	free_memory(params.cmds, params.paths);
}
