/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:16:55 by analexan          #+#    #+#             */
/*   Updated: 2023/10/12 17:59:25 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(int n)
{
	if (!n)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n");
	else if (n == 1)
		prt("PATH not found\n");
	else
	{
		free_strs(vars()->paths);
		free_strs(vars()->cmdargs2);
		free_strs(vars()->cmdargs3);
	}
	if (n == 2)
		perror("pipex");
	else if (n == 3)
		exit(127);
	exit(EXIT_FAILURE);
}

char	*search_cmd(char **cmdargs)
{
	int		i;
	char	*cmd;

	i = -1;
	if (!ft_strchr(cmdargs[0], '/'))
	{
		while (vars()->paths[++i])
		{
			cmd = ft_strjoin(vars()->paths[i], cmdargs[0]);
			if (!access(cmd, F_OK | X_OK))
				return (cmd);
			free(cmd);
		}
	}
	else
		if (!access(cmdargs[0], F_OK | X_OK))
			return (ft_strdup(cmdargs[0]));
	return (NULL);
}

void	process(char **cmdargs, char **av, char **ep, int mode)
{
	char	*cmd;
	int		fd;
	int		dup_fd[2];

	if (!mode)
		fd = open(av[1], O_RDONLY);
	else
		fd = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		error(2);
	dup_fd[0] = dup2(fd, mode);
	dup_fd[1] = dup2(vars()->end[!mode], !mode);
	if (dup_fd[0] < 0 || dup_fd[1] < 0)
		error(2);
	close(vars()->end[1]);
	close(vars()->end[0]);
	close(fd);
	cmd = search_cmd(cmdargs);
	if (cmd)
		execve(cmd, cmdargs, ep);
	free(cmd);
	close(dup_fd[0]);
	close(dup_fd[1]);
	perror(cmdargs[0]);
	error(3);
}

void	parsing(char **ep)
{
	char	*path_from_ep;
	char	*temp;
	int		i;

	i = -1;
	while (ep[++i])
	{
		path_from_ep = ft_strnstr(ep[i], "PATH=", 5);
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
		(vars()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
	}
}

// to-do: fix leaks, etc...
int	main(int ac, char **av, char **ep)
{
	pid_t	child2;
	pid_t	child1;

	if (ac != 5)
		error(0);
	parsing(ep);
	vars()->cmdargs2 = ft_split(av[2], ' ');
	vars()->cmdargs3 = ft_split(av[3], ' ');
	if (pipe(vars()->end) == -1)
		error(2);
	child1 = fork();
	if (child1 < 0)
		error(2);
	if (!child1)
		process(vars()->cmdargs2, av, ep, 0);
	child2 = fork();
	if (child2 < 0)
		error(2);
	if (!child2)
		process(vars()->cmdargs3, av, ep, 1);
	close(vars()->end[0]);
	close(vars()->end[1]);
	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
	return (0);
}
