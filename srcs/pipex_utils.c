/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 18:36:21 by analexan          #+#    #+#             */
/*   Updated: 2023/10/18 19:07:05 by analexan         ###   ########.fr       */
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
