/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:16:55 by analexan          #+#    #+#             */
/*   Updated: 2023/09/22 17:39:53 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
void	parent_process(int f2, char *cmd2)
{
	int status;

	waitpid(-1, &status, 0);
	// dup2(f2, ...); // f2 is the stdout
	// dup2(end[0], ...); // end[0] is the stdin
	// close(end[1])
	// close(f2);
	// execve function for each possible path
	exit(EXIT_FAILURE);
}

void	child_process(int f1, char *cmd1)
{
	// add protection if dup2() < 0
	// dup2 close stdin, f1 becomes the new stdin
	dup2(f1, STDIN_FILENO); // we want f1 to be execve() input
	// dup2(end[1], STDOUT_FILENO); // we want end[1] to be execve() stdout
	// close(end[0]) 
	/* always close the end of the pipe you don't use,
						as long as the pipe is open, the other end will 
						be waiting for some kind of input and will not
						be able to finish its process */
	// close(f1);
	// execve function for each possible path
	exit(EXIT_FAILURE);
}
void    pipex(int f1, int f2, char **cmd, char **envp)
{
	int   end[2];
	pid_t parent;
	pipe(end);
	parent = fork();
	if (parent < 0)
			return (perror("Fork: "));
	if (!parent) // if fork() returns 0, we are in the child process
		child_process(f1, cmd[1]);
	else
		parent_process(f2, cmd[2]);
}

// char **ep = environment pointer
int	main(int ac, char **av, char **envp)
{
	ac = 5;
	av[1] = "infile"; // file1
	av[2] = "ls -l"; // cmd1
	av[3] = "wc -l"; // cmd2
	av[4] = "outfile"; // file2

	
	// int	f1;
	// int	f2;
	// f1 = open(av[1], O_RDONLY);
	// f2 = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	// if (f1 < 0 || f2 < 0)
	// 	return (-1);
	// pipex(f1, f2, av, envp);
	return (0);
}
