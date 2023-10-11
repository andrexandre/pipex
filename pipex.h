/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:17:18 by analexan          #+#    #+#             */
/*   Updated: 2023/10/11 18:50:50 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
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
// va_args
# include <stdarg.h>

// mandatory
typedef struct s_vars
{
	char	**cmdargs2;
	char	**cmdargs3;
	int		end[2];
	char	**paths;

	char	***cmdargs;
	int		**pipe;
	int		ac;
	pid_t	*pids;
}			t_vars;
// bonus

// ft_split
char	**ft_split(char const *s, char c);
void	*free_strs(char **strs);

// tool_lib
void	prt(char *string, ...);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);

// tool_lib2
size_t	ft_strlen(char const *s);
char	*ft_strnstr(const char *big, const char *little, size_t len);
t_vars	*vars(void);
void	*ft_calloc(size_t nmemb, size_t size);

#endif