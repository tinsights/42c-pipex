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
	if (ac != 5)
		return printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");


	char *bin_path = "/bin/bash";

	char *file = ft_strjoin(av[1], " ");
	char *cmd = ft_strjoin(file, av[2]);
	char *leftpipe = ft_strjoin("< ", cmd);
	free(file);
	free(cmd);

	file = ft_strjoin(av[3], " > ");
	cmd = ft_strjoin(file, av[4]);
	free(file);

	file = ft_strjoin(" | ", cmd);
	free(cmd);

	char *bashcmd = ft_strjoin(leftpipe, file);
	free(leftpipe);

	char *bashargs[] = { bin_path, "-c", bashcmd, NULL};
	execve(bin_path, bashargs, envp);
	perror("");
}
