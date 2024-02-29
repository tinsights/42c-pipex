/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:11:37 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/28 21:11:41 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t size)
{
	void	*res;
	size_t	maxcpy;

	if (!size)
	{
		if (ptr)
			free (ptr);
		return (NULL);
	}
	if (!ptr)
		return (ft_calloc(1, size));
	res = ft_calloc(1, size);
	if (size >= old_size)
		maxcpy = old_size;
	else
		maxcpy = size;
	ft_memmove(res, ptr, maxcpy);
	free(ptr);
	return (res);
}

void	add_to_argv(char ***argv, char *str)
{
	int		num_strs;
	size_t	size;

	num_strs = 0;
	while (*argv && (*argv)[num_strs] != NULL)
		num_strs++;
	size = (num_strs + 1) * sizeof(char *);
	*argv = (char **) ft_realloc(*argv, size, size + sizeof(char *));
	(*argv)[num_strs] = str;
	(*argv)[num_strs + 1] = NULL;
}

void process_subquote()
{
	int qlen = 0;
	int x;
	char *cmd;
	char *qarg;
	char **args;

	if (qstart != arg)
	{
		cmd = ft_substr(arg, 0, qstart - arg);
		args = ft_split(cmd, ' ');
		free(cmd);
		x = 0;
		while (args[x])
			add_to_argv(argv, args[x++]);
		free(args);
	}
	while (qstart[qlen + 1] && qstart[qlen + 1] != qchar)
		qlen++;
	if (qlen)
	{
		qarg = ft_substr(qstart, 1, qlen + 1);
		add_to_argv(argv, qarg);
		qarg[qlen] = 0;
	}
	if (j + qlen < ft_strlen(arg))
		separate_quotes(arg + j + qlen + 2, argv);
}

void	separate_quotes(char *arg, char ***argv)
{
	// int totallen = ft_strlen(arg);
	char *qstart = NULL;
	char qchar = 0;
	int j = -1;
	while (arg[++j])
	{
		if (arg[j] == '\'' || arg[j] == '"')
		{
			qstart = arg + j;
			qchar = arg[j];
			break;
		}
	}
	if (qstart)
	{
		int qlen = 0;
		char *cmd;
		if (qstart != arg)
		{
			cmd = ft_substr(arg, 0, qstart - arg);
			char **args = ft_split(cmd, ' ');
			free(cmd);
			int x = 0;
			while (args[x])
				add_to_argv(argv, args[x++]);
			free(args);
		}
		char *qarg;
		while (qstart[qlen + 1] && qstart[qlen + 1] != qchar)
			qlen++;

		if (qlen)
		{
			qarg = ft_substr(qstart, 1, qlen + 1);
			add_to_argv(argv, qarg);
			qarg[qlen] = 0;
		}
		if (j + qlen < ft_strlen(arg))
			separate_quotes(arg + j + qlen + 2, argv);
	}
	else
	{
		char **args = ft_split(arg, ' ');
		int x = 0;
		while (args[x])
			add_to_argv(argv, args[x++]);
		free(args);
	}
}
