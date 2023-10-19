/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 18:36:21 by analexan          #+#    #+#             */
/*   Updated: 2023/10/19 20:09:22 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

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

void	close_all(int fd1, int fd2)
{
	int		i;

	if (fd1 > -1)
		close(fd1);
	if (fd2 > -1)
		close(fd2);
	i = -1;
	while (++i < vars()->ac - 4)
	{
		close(vars()->pipe[i][0]);
		close(vars()->pipe[i][1]);
	}
}

void	error_b(int n)
{
	if (n == -1)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n");
	else if (!n)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> ... <file2>\n");
	else if (n == 1)
		free_all(1);
	else
		free_all(0);
	if (n > 1)
		perror("pipex");
	else if (n == 3)
	{
		if (!vars()->av[1] || !*vars()->av[1])
			perror("''");
		else
			perror(vars()->av[1]);
	}
	exit(EXIT_FAILURE);
}

// WIP
void	here_doc(int ac, char **av)
{
	int		fd;
	char	*buf;

	if (ac != 6)
		error_b(0);
	fd = open(av[ac - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(av[ac - 1]);
		exit(EXIT_FAILURE);
	}
	buf = get_next_line(0);
	printf("%s\n", buf);
	while (ft_strcmp(buf, ft_strjoin(av[2], "\n")))
	{
		write(fd, buf, ft_strlen(buf));
		free(buf);
		buf = get_next_line(0);
		printf("%s\n", buf);
	}
	free(buf);
	close(fd);
	exit(EXIT_SUCCESS);
}

void	check_fds(int ac, char **av)
{
	int		fd;
	int		fd2;

	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		perror(av[1]);
	else
		close(fd);
	fd2 = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd2 == -1)
		perror(av[ac - 1]);
	else
		close(fd2);
	if (fd == -1 || fd2 == -1)
		exit(EXIT_FAILURE);
}
