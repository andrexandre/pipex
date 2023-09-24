// open, close, read, write,
// access, dup, dup2, execve,
// exit, fork, pipe, unlink:
# include <unistd.h>
// malloc, free:
# include <stdlib.h>
// perror, strerror:
# include <stdio.h>
// wait, waitpid:
# include <sys/wait.h>
// flags:
# include <fcntl.h>

//make 2 child and make them sum and then return to main
// id == 0 is child
// id != 0 is parent
#define PARENT id && id2
#define CHILD !id && id2
#define CHILD2 !id && !id2
int	main(void)
{
	int   pfd[2];

	if (pipe(pfd) == -1)
		return 1;
	int id = fork();
	int id2 = -2;
	if (!id)
		id2 = fork();
	if (id2 == -1)
		return 2;
	if (PARENT)
		printf("IMA PARENT\n");
	if (CHILD)
		printf("IMA CHILD\n");
	if (CHILD2)
		printf("IMA CHILD TOO\n");
	wait(NULL);
	printf("LET'S GOO\n");
	// int	iarr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	// int iarr_s = sizeof(iarr) / sizeof(int);
	// int start = 0;
	// int end = 0;
	// int sum = 0;
	// if (!id)
	// {
	// 	start = 0;
	// 	end = iarr_s / 2;
	// }
	// else if (!id2)
	// {
	// 	start = iarr_s / 2;
	// 	end = iarr_s;
	// }
	// int i = start;
	// while (!id2 && i < end)
	// {
	// 	sum += iarr[i];
	// 	i++;
	// }
	// if (id2)
	// 	wait(NULL);
	// printf("%i/%i/%i\n", id, id2, sum);
	// if (!id) // is_child
	// {
	// 	close(pfd[0]);
	// 	if (write(pfd[1], &sum, sizeof(int)) == -1)
	// 		return 3;
	// 	close(pfd[1]);
	// }
	// else if (!id2)
	// {
	// 	int sum_sibling;
	// 	// close(pfd[1]);
	// 	if (read(pfd[0], &sum_sibling, sizeof(int)) == -1)
	// 		return 4;
	// 	printf("sum: %i\n", sum_sibling);
	// 	close(pfd[0]);
	// 	sum += sum_sibling;
	// 	if (write(pfd[1], &sum, sizeof(int)) == -1)
	// 		return 3;
	// 	close(pfd[1]);
	// }
	// if (id2)
	// {
	// 	int final_sum;
	// 	close(pfd[1]);
	// 	if (read(pfd[0], &final_sum, sizeof(int)) == -1)
	// 		return 4;
	// 	close(pfd[0]);
	// 	printf("f. sum: %i\n", final_sum);
	// }
	return 0;
	// 10+26=36
}
// fd[0] = read
// fd[1] = write