/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:16:55 by analexan          #+#    #+#             */
/*   Updated: 2023/10/05 19:05:24 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(int n)
{
	printf("Error: %i\n", n);
	exit(EXIT_FAILURE);
}

	// querias(ac, av);
void	querias(int ac, char **av)
{
	char	str[100];

	if (ac == 5)
		snprintf(str, sizeof(str), "< %s %s | %s > %s",
			av[1], av[2], av[3], av[4]);
	else
	{
		str[0] = 'e';
		str[1] = 'c';
		str[2] = 'h';
		str[3] = 'o';
		str[4] = ' ';
		str[5] = 'n';
		str[6] = 'o';
	}
	system(str);
}

void	parent_process(int f2, char *cmd2, int *end, char **envp)
{
	char	str[50];
	int		status;

	waitpid(-1, &status, 0);
	dup2(f2, STDOUT_FILENO);
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	close(f2);
	snprintf(str, sizeof(str), "/usr/bin/%s", cmd2);
	execve(str, (char *const []){"-l", NULL }, envp);
	exit(EXIT_FAILURE);
}
	// execve function for each possible path

	// add protection if dup2() < 0
void	child_process(int f1, char *cmd1, int *end, char **envp)
{
	char	str[50];

	dup2(f1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(end[0]);
	close(f1);
	snprintf(str, sizeof(str), "/usr/bin/%s", cmd1);
	execve(str, (char *const []){"-e", NULL }, envp);
	exit(EXIT_FAILURE);
}
	// execve function for each possible path

void	pipex(int f1, int f2, char **cmd, char **envp)
{
	int		end[2];
	pid_t	parent;

	(void)envp;
	pipe(end);
	parent = fork();
	if (parent < 0)
		return (perror("Fork: "));
	if (!parent)
		child_process(f1, cmd[2], end, envp);
	else
		parent_process(f2, cmd[3], end, envp);
}

// char **ep = environment pointer
int	main(int ac, char **av, char **envp)
{
	int	f1;
	int	f2;

	if (ac < 5)
		return (1);
	f1 = open(av[1], O_RDONLY);
	f2 = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (f1 < 0 || f2 < 0)
		return (1);
	pipex(f1, f2, av, envp);
	return (0);
}
	// ac = 5;
	// av[2] = "cat"; // cmd1
	// av[1] = "infile"; // file1
	// av[2] = "ls -l"; // cmd1
	// av[3] = "wc -l"; // cmd2
	// av[4] = "outfile"; // file2
