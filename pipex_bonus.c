/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:16:55 by analexan          #+#    #+#             */
/*   Updated: 2023/10/11 19:03:49 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(int n)
{
	int	i;

	if (!n)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n");
	else if (n == 1)
		prt("PATH not found\n");
	else
	{
		free_strs(vars()->paths);
		i = -1;
		while (++i < vars()->ac - 3)
			free_strs(vars()->cmdargs[i]);
		free(vars()->cmdargs);
		i = -1;
		while (++i < vars()->ac - 4)
			free(vars()->pipe[i]);
		free(vars()->pipe);
		free(vars()->pids);
		// free all pids
	}
	if (n == 2)
		perror("pipex");
	else if (n == 3)
		exit(127);
	exit(EXIT_FAILURE);
}

void	process(char **cmdargs, char **av, char **ep, int mode)
{
	char	*cmd;
	int		i;
	int		status;
	int		fd;

	wait(&i);
	if (!mode)
		fd = open(av[1], O_RDONLY);
	else
		fd = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		error(2);
	i = -1;
	if (dup2(fd, mode) < 0 || dup2(vars()->pipe[0][!mode], !mode) < 0)
		error(2);
	close(vars()->pipe[0][mode]);
	close(fd);
	while (vars()->paths[++i])
	{
		cmd = ft_strjoin(vars()->paths[i], cmdargs[0]);
		if (!access(vars()->paths[i], F_OK | X_OK))
			execve(cmd, cmdargs, ep);
		free(cmd);
	}
	perror(cmdargs[0]);
	error(3);
}
// execve("/usr/bin/ls", {"ls", "-l", NULL}, {"PATH=/usr/bin", NULL});

void	fill_args_n_pipe(int ac, char **av)
{
	int	i;

	vars()->cmdargs = ft_calloc(ac - 2, sizeof(char *));
	vars()->cmdargs[ac - 3] = NULL;
	i = -1;
	while (++i < ac - 3)
		vars()->cmdargs[i] = ft_split(av[i + 2], ' ');
	vars()->pipe = ft_calloc(ac - 4, sizeof(int *));
	i = -1;
	while (++i < ac - 4)
	{
		vars()->pipe[i] = ft_calloc(2, sizeof(int));
		if (pipe(vars()->pipe[i]) == -1)
			error(2);
	}
}

// temp2 is only for norm error
void	parsing(char **ep)
{
	char	*path_from_ep;
	char	*temp;
	char	**temp2;
	int		i;

	i = -1;
	while (ep[++i])
	{
		path_from_ep = ft_strnstr(ep[i], "PATH=/nfs", ft_strlen(ep[i]));
		if (path_from_ep)
			break ;
	}
	if (!ep[i])
		error(1);
	path_from_ep += 5;
	vars()->paths = ft_split(path_from_ep, ':');
	i = -1;
	while (vars()->paths[++i])
	{
		temp = vars()->paths[i];
		temp2 = &vars()->paths[i];
		*temp2 = ft_strjoin(temp, "/");
		free(temp);
	}
}

// in this moment make all is make bonus
int	main(int ac, char **av, char **ep)
{
	if (ac < 5)
		error(0);
	vars()->ac = ac;
	parsing(ep);
	fill_args_n_pipe(ac, av);
	vars()->pids = ft_calloc(ac - 4, sizeof(int *));
	vars()->pids[0] = fork();
	if (vars()->pids[0] < 0)
		error(2);
	if (!vars()->pids[0])
		process(vars()->cmdargs[0], av, ep, 0);
	else
		process(vars()->cmdargs[1], av, ep, 1);
	return (0);
}
/*
		// int a = -1;
		// while (cmdargs[++a])
		// 	prt("'%s'\n", cmdargs[a]);
*/