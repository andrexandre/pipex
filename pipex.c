/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:16:55 by analexan          #+#    #+#             */
/*   Updated: 2023/10/10 19:21:46 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(int n)
{
	if (!n)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n");
	else if (n == 1)
		prt("PATH not found\n");
	else if (n == 2)
		perror("pipex");
	else
		prt("Error: %i\n", n);
	exit(EXIT_FAILURE);
}

void	process(char **cmdargs, char **paths, char **av, int mode)
{
	char	*cmd;
	int		i;
	int		fd;

	waitpid(-1, &i, 0);
	if (!mode)
		fd = open(av[1], O_RDONLY);
	else
		fd = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		error(2);
	i = -1;
	if (dup2(fd, mode) < 0 || dup2(vars()->end[!mode], !mode) < 0)
		error(2);
	close(vars()->end[mode]);
	close(fd);
	while (paths[++i])
	{
		cmd = ft_strjoin(paths[i], cmdargs[0]);
		if (!access(paths[i], F_OK | X_OK))
			execve(cmd, cmdargs, vars()->ep);
		free(cmd);
	}
	perror(cmdargs[0]);
	exit(127);
}
// execve("/usr/bin/ls", {"ls", "-l", NULL}, {"PATH=/usr/bin", NULL});

char	**parsing(char **av, char **ep)
{
	char	**paths;
	char	*path_from_ep;
	int		i;

	i = -1;
	paths = NULL;
	while (ep[++i])
	{
		path_from_ep = ft_strnstr(ep[i], "PATH=/nfs", ft_strlen(ep[i]));
		if (path_from_ep)
			break ;
	}
	if (!ep[i])
		error(1);
	path_from_ep += 5;
	paths = ft_split(path_from_ep, ':');
	i = -1;
	while (paths[++i])
		paths[i] = ft_strjoin(paths[i], "/");
	vars()->cmdargs2 = ft_split(av[2], ' ');
	vars()->cmdargs3 = ft_split(av[3], ' ');
	vars()->ep = ep;
	return (paths);
}

// char **ep = environment pointer
int	main(int ac, char **av, char **ep)
{
	pid_t	parent;
	char	**paths;

	if (ac != 5)
		error(0);
	paths = parsing(av, ep);
	pipe(vars()->end);
	parent = fork();
	if (parent < 0)
		error(2);
	if (!parent)
		process(vars()->cmdargs2, paths, av, 0);
	else
		process(vars()->cmdargs3, paths, av, 1);
	return (0);
}
	// free_strs(vars()->paths);
	// free_strs(vars()->cmdargs2);
	// free_strs(vars()->cmdargs3);
/*
		// int a = -1;
		// while (cmdargs[++a])
		// 	prt("'%s'\n", cmdargs[a]);

	// ac = 5;
	// av[2] = "cat"; // cmd1
	// av[1] = "infile"; // file1
	// av[2] = "ls -l"; // cmd1
	// av[3] = "wc -l"; // cmd2
	// av[4] = "outfile"; // file2
*/
	// querias(ac, av);
// void	querias(int ac, char **av)
// {
// 	char	str[100];
// 	if (ac == 5)
// 		snprintf(str, sizeof(str), "< %s %s | %s > %s",
// 			av[1], av[2], av[3], av[4]);
// 	else
// 	{
// 		str[0] = 'e';
// 		str[1] = 'c';
// 		str[2] = 'h';
// 		str[3] = 'o';
// 		str[4] = ' ';
// 		str[5] = 'n';
// 		str[6] = 'o';
// 	}
// 	system(str);
// }
