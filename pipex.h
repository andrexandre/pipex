/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:17:18 by analexan          #+#    #+#             */
/*   Updated: 2023/09/22 13:55:25 by analexan         ###   ########.fr       */
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
#endif