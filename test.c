
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>


void *ft_realloc(void *ptr, size_t old_size, size_t size)
{
	void *res;
	size_t maxcpy;
	if (!size)
	{
		if (ptr)
			free (ptr);
		return (NULL);
	}
	if (!ptr)
		return ft_calloc(1, size);

	res = ft_calloc(1, size);
	if (size >= old_size)
		maxcpy = old_size;
	else
		maxcpy = size;
	ft_memmove(res, ptr, maxcpy);
	free(ptr);
	return res;
}

void add_to_argv(char ***argv, char *str)
{
	int num_strs;
	size_t size;

	num_strs = 0;
	while ((*argv)[num_strs] != NULL)
		num_strs++;
	size = (num_strs + 1) * sizeof(char *);
	*argv = (char **) ft_realloc(*argv, size, size +  sizeof(char *));
	(*argv)[num_strs] = str;
	(*argv)[num_strs + 1] = NULL;
}

void separate_quotes(char *arg, char ***argv)
{
	// ft_printf("arg in rec: %s\n", arg);
	int i = 0;
	int totallen = ft_strlen(arg);

	// ft_printf("total len of %s is %i\n", arg, totallen);

	int j = 0;
	char *qstart = NULL;
	char qchar = 0;
	char c;
	while (arg[j])
	{
		c = arg[j];
		if (c == '\'' || c == '"')
		{
			qstart = arg + j;
			qchar = c;
			break;
		}
		j++;
	}
	if (qstart)
	{
		// ft_printf("qchar is %c\n", qchar);
		int qlen = 0;
		char *cmd;
		if (qstart != arg)
		{
			cmd = ft_substr(arg, 0, qstart - arg);
			char **args = ft_split(cmd, ' ');
			free(cmd);
			int x = 0;
			while (args[x])
			{
				add_to_argv(argv, args[x]);
				// ft_printf("%s\n", *(args));
				// free(*args);
				x++;
			}
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
		// ft_printf("%s\n", qarg);
		// ft_printf("%i %i\n ", j + qlen + 2, totallen);
		if (j + qlen < totallen)
			separate_quotes(arg + j + qlen + 2, argv);
	}
	else
	{
		char **args = ft_split(arg, ' ');
		int x = 0;
		while (args[x])
		{
			add_to_argv(argv, args[x]);
			// ft_printf("%s\n", *(args));
			x++;
		}
		free(args);
	}
}


int main(int ac, char **av)
{
	if (ac == 1)
		return ft_printf("oen arg req\n");

	int i = 1;
	char **argv = ft_calloc(1, sizeof(char*));
	while (av[i])
	{
		separate_quotes(av[i], &argv);
		i++;
	}
	i = 0;
	while (argv[i])
	{
		ft_printf("%s\n", argv[i]);
		free(argv[i]);
		i++;
	}
	free(argv);
}