/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:16:55 by analexan          #+#    #+#             */
/*   Updated: 2023/10/16 19:14:18 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(int n)
{
	int	i;

	free_strs(vars()->paths);
	i = -1;
	while (++i < vars()->ac - 3)
		free_strs(vars()->cmdargs[i]);
	free(vars()->cmdargs);
	i = -1;
	while (++i < vars()->ac - 4)
		free(vars()->pipe[i]);
	free(vars()->pipe);
	if (n)
		return ;
	free(vars()->pids);
}

void	error(int n)
{
	if (!n)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> ... <file2>\n");
	else if (n == 1)
		prt("PATH not found\n");
	else if (n == 2)
		free_all(1);
	else
		free_all(0);
	if (n > 1)
		perror("pipex");
	exit(EXIT_FAILURE);
}

char	*search_cmd(char **cmdargs, int fd)
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
	perror(cmdargs[0]);
	close(vars()->pipe[0][1]);
	close(vars()->pipe[0][0]);
	close(fd);
	free_all(0);
	exit(127);
}

// else is just for showing purpuses
void	process(char **cmdargs, char **av, char **ep, int mode)
{
	char	*cmd;
	int		fd;

	if (!mode)
		fd = vars()->fd1;
	else if (mode == 1)
		fd = vars()->fd2;
	else
		fd = vars()->pipe[1][!mode];
	cmd = search_cmd(cmdargs, fd);
	if (dup2(fd, mode) < 0 || dup2(vars()->pipe[0][!mode], !mode) < 0)
		error(3);
	close(vars()->pipe[0][1]);
	close(vars()->pipe[0][0]);
	close(fd);
	execve(cmd, cmdargs, ep);
	free(cmd);
	perror(cmdargs[0]);
	free_all(0);
	exit(127);
}

void	fill_args_n_pipe(int ac, char **av, char **ep)
{
	int	i;

	vars()->cmdargs = ft_calloc(ac - 2, sizeof(char *));
	vars()->cmdargs[ac - 3] = NULL;
	i = -1;
	while (++i < ac - 3)
		(vars()->cmdargs[i]) = ft_split(av[i + 2], ' ');
	vars()->pipe = ft_calloc(ac - 4, sizeof(int *));
	i = -1;
	while (++i < ac - 4)
	{
		(vars()->pipe[i]) = ft_calloc(2, sizeof(int));
		if (pipe(vars()->pipe[i]) == -1)
			error(2);
	}
	vars()->pids = ft_calloc(ac - 3, sizeof(int));
	i = -1;
	vars()->fd1 = open(av[1], O_RDONLY);
	if (vars()->fd1 < 0)
		error(3);
	vars()->fd2 = open(av[vars()->ac - 1],
		O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (vars()->fd2 < 0)
		error(3);
	while (++i < ac - 3)
	{
		vars()->pids[i] = fork();
		if (vars()->pids[i] < 0)
			error(3);
		if (!vars()->pids[i])
			process(vars()->cmdargs[i], av, ep, i);
	}
}
		// close pipes

void	parsing(char **ep, char **av, int i)
{
	char	*path_from_ep;
	char	*temp;

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
	if (!vars()->paths)
		exit(EXIT_FAILURE);
	i = -1;
	while (vars()->paths[++i])
	{
		temp = vars()->paths[i];
		(vars()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!vars()->paths[i])
			exit(EXIT_FAILURE);
	}
}
/*
make && valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all 
./pipex infile cat wc outfile
< infile cat | wc > outfile
*/

// close open fds and fix bonus
int	main(int ac, char **av, char **ep)
{
	if (ac < 5)
		error(0);
	vars()->ac = ac;
	parsing(ep, av, -1);
	fill_args_n_pipe(ac, av, ep);
	close(vars()->pipe[0][0]);
	close(vars()->pipe[0][1]);
	close(vars()->fd1);
	close(vars()->fd2);
	waitpid(vars()->pids[0], NULL, 0);
	waitpid(vars()->pids[1], NULL, 0);
	free_all(0);
	return (0);
}

/*
commands to test:
./pipex infile "ping google.com -c 5" "wc -c" outfile
./pipex  cat cat outfile
./pipex  0 cat cat outfile
./pipex "NULL" cat cat outfile
unset PWD
./pipex pipex.c pwd cat outfiler
./pipex pipex.c "echo $PWD" cat outfiler
./pipex pipex.c "echo $OLDPWD" cat outfiler
< pipex.c "echo $PWD" | cat > outfiler
valgrind --trace-children=yes ./pipex pipex.c pwd cat outfile
*/