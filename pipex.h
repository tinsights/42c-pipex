/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:12:01 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/28 21:12:04 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

# ifndef BONUS
#  define BONUS 0
# endif

typedef struct s_params
{
	int		num_cmds;
	char	**paths;
	char	***cmds;
	char	*delim;
	int		fd[3];

}	t_params;

void	separate_quotes(char *arg, char ***argv);
int		init_data(int ac, char **av, t_params *p, char **envp);
int		init_heredoc(int ac, char **av, t_params *p, char **envp);
void	free_memory(char ***cmds, char **paths);

#endif