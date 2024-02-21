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
#include <errno.h>
#include "libft.h"

int	main(int ac, char **av, char **envp)
{
	char	*file;
	char	*cmd;
	char	*leftpipe;
	char	*bashcmd;
	

	if (ac != 5)
		return (printf("Usage: ./pipex file1 cmd1 cmd2 file2\n"));
	file = ft_strjoin(av[1], " ");
	cmd = ft_strjoin(file, av[2]);
	leftpipe = ft_strjoin("< ", cmd);
	free(file);
	free(cmd);
	file = ft_strjoin(av[3], " > ");
	cmd = ft_strjoin(file, av[4]);
	free(file);
	file = ft_strjoin(" | ", cmd);
	free(cmd);
	bashcmd = ft_strjoin(leftpipe, file);
	free(leftpipe);
	char	*bashargs[] = {"/bin/bash", "-c", bashcmd, NULL};
	execve("/bin/bash", bashargs, envp);
	perror("");
	return (errno);
}
