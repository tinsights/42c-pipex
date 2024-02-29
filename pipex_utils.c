/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:57:10 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/29 16:57:10 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	open_files(int *ac, char ***av, t_params *p)
{
	int	p_fd[2];

	if (BONUS && !ft_strncmp((*av)[1], "here_doc", 9))
	{
		p->fd[1] = open((*av)[(*ac) - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (p->fd[1] < 0)
			perror((*av)[(*ac)-- - 1]);
		pipe(p_fd);
		p->fd[0] = p_fd[0];
		p->fd[2] = p_fd[1];
	}
	else
	{
		p->fd[1] = open((*av)[*ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (p->fd[1] < 0)
			perror((*av)[(*ac)-- - 1]);
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
		p->paths = (char **)(ft_calloc(1, sizeof(char *)));
	return (1);
}

int	init_heredoc(int ac, char **av, t_params *p, char **envp)
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
		p->paths = (char **)(ft_calloc(1, sizeof(char *)));
	return (1);
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