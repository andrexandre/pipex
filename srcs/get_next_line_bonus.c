/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 14:44:07 by analexan          #+#    #+#             */
/*   Updated: 2023/10/20 13:31:27 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

size_t	gnl_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != '\n')
		i++;
	return (i + (s[i] == '\n'));
}

void	ft_bzero(void *s, size_t n)
{
	unsigned int	i;
	char			*ptr;

	i = 0;
	ptr = (char *)s;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	str = (char *)malloc(gnl_strlen(s1) + gnl_strlen(s2) + 1);
	if (!str)
		return (NULL);
	i = -1;
	if (s1)
		while (s1[++i])
			str[i] = s1[i];
	if (i == -1)
		i = 0;
	j = 0;
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
		if (s2[j - 1] == '\n')
			break ;
	}
	str[i + j] = 0;
	return (str);
}

int	check_buffer(char *str)
{
	int	nl;
	int	i;
	int	j;

	nl = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (nl)
			str[j++] = str[i];
		if (str[i] == '\n')
			nl = 1;
		str[i++] = 0;
	}
	return (nl);
}

char	*get_next_line(int fd)
{
	static char	buffer[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*line;
	char		*temp;

	if (read(fd, 0, 0) || BUFFER_SIZE < 1)
	{
		if (fd >= 0 && fd <= FOPEN_MAX)
			ft_bzero(buffer[fd], BUFFER_SIZE);
		return (NULL);
	}
	line = NULL;
	while (buffer[fd][0] != 0 || read(fd, buffer[fd], BUFFER_SIZE))
	{
		temp = line;
		line = ft_strjoin_gnl(temp, buffer[fd]);
		free(temp);
		if (check_buffer(buffer[fd]))
			break ;
	}
	return (line);
}
/*
#include "get_next_line_utils_bonus.c"
// cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 *.c && ./a.out
#include <fcntl.h>
void	gnl_tester(char *file, int lcnt)
{
	int		fd;
	char	*str;
	int		i;

	i = 1;
	if (!file || lcnt < 0)
		return ;
	fd = open(file, O_RDONLY);
	while (lcnt == 0 && (str = get_next_line(fd)))
	{
		printf("%i:\"%s\"\n", i, str);
		free(str);
		i++;
	}
	while (lcnt != 0 && i <= lcnt)
	{
		str = get_next_line(fd);
		printf("%i:\"%s\"\n", i, str);
		free(str);
		i++;
	}
	close(fd);
}
int main(void)
{
	gnl_tester("lines.txt", 0);
	return (0);
}
*/