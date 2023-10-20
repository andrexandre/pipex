/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 18:36:21 by analexan          #+#    #+#             */
/*   Updated: 2023/10/20 19:04:17 by analexan         ###   ########.fr       */
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
	// while (++i < vars()->ac - 5) // 5 for bonus

void	error_b(int n)
{
	if (n == -1)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n");
	else if (!n)
		prt("usage: ./pipex <file1> <cmd1> <cmd2> ... <file2>\n");
	else if (n == 1)
		free_all(1);
	else if (n == 4)
		prt("usage: ./pipex here_doc EOF <cmd1> <cmd2> <file>\n");
	else
		free_all(0);
	if (n == 3)
	{
		if (!vars()->av[1] || !*vars()->av[1])
			perror("''");
		else
			perror(vars()->av[1]);
	}
	else if (n > 1 && n != 4)
		perror("pipex");
	exit(EXIT_FAILURE);
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
/*
prt("usage: ./pipex <file1> <cmd1> <cmd2> ... <file2>\n");
5 - 2 = 3 cmds + NULL
5 - 3 = 2 cmds
5 - 4 = 1 pipe from c1 to c2
5 - 3 = 2 child
prt("usage: ./pipex here_doc EOF <cmd1> <cmd2> <file>\n");
6 - 3 = 3 cmds + NULL
6 - 4 = 2 cmds
6 - 5 = 1 pipe
6 - 4 = 2 child
*/

int	get_stdout(void)
{
	char	*buf;
	int		fd;
	char	*str;

	fd = vars()->pipe[0][1];
	if (dup2(fd, STDIN_FILENO) < 0)
		error_b(2);
	prt("pipe heredoc>");
	buf = get_next_line(STDIN_FILENO);
	str = ft_strjoin(vars()->av[2], "\n");
	while (ft_strcmp(buf, str))
	{
		write(fd, buf, ft_strlen(buf));
		free(buf);
		prt("pipe heredoc>");
		buf = get_next_line(STDIN_FILENO);
	}
	free(str);
	return (fd);
}

void	process_hd(char **cmdargs, char **av, char **ep, int mode)
{
	char	*cmd;
	int		fd;
	int		fd2;

	prt("n\n");
	if (!mode)
		fd = get_stdout();
	else
		fd = vars()->pipe[mode - 1][0];
	if (mode == vars()->ac - 4)
		fd2 = open(av[vars()->ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd2 = vars()->pipe[mode][1];
	if (fd < 0 || fd2 < 0)
		error_b(3);
	cmd = search_cmd(cmdargs, fd, fd2);
	if ((mode && dup2(fd, STDIN_FILENO) < 0) || dup2(fd2, STDOUT_FILENO) < 0)
		error_b(2);
	close_all(fd, fd2);
	execve(cmd, cmdargs, ep);
	free(cmd);
	perror(cmdargs[0]);
	free_all(0);
	exit(127);
}

// cmd << LIMITER | cmd1 >> outfile
void	fill_args_n_pipe_hd(int ac, char **av, char **ep)
{
	int	i;

	vars()->cmdargs = ft_calloc(ac - 3, sizeof(char *));
	vars()->cmdargs[ac - 4] = NULL;
	i = -1;
	while (++i < ac - 4)
		(vars()->cmdargs[i]) = ft_split(av[i + 3], ' ');
	vars()->pipe = ft_calloc(ac - 5, sizeof(int *));
	i = -1;
	while (++i < ac - 5)
	{
		(vars()->pipe[i]) = ft_calloc(2, sizeof(int));
		if (pipe(vars()->pipe[i]) == -1)
			error_b(1);
	}
	vars()->pids = ft_calloc(ac - 4, sizeof(int));
	i = -1;
	while (++i < ac - 4)
	{
		vars()->pids[i] = fork();
		if (vars()->pids[i] < 0)
			error_b(2);
		if (!vars()->pids[i])
			process_hd(vars()->cmdargs[i], av, ep, i);
	}
}

void	here_doc(int ac, char **av, char **ep)
{
	int	i;

	if (ac != 6)
		error_b(4);
	parsing(ep, -1);
	fill_args_n_pipe_hd(ac, av, ep);
	close_all(-1, -1);
	i = -1;
	while (++i < ac - 4)
		waitpid(vars()->pids[i], NULL, 0);
	free_all(0);
	exit(EXIT_SUCCESS);
}
	// printf("s: %s\n", buf);
