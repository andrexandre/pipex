/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_lib2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 17:16:37 by analexan          #+#    #+#             */
/*   Updated: 2023/10/09 18:02:55 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	ft_strlen(char const *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	unsigned int	i;
	int				j;
	char			*bptr;
	char			*lptr;

	bptr = (char *)big;
	lptr = (char *)little;
	i = 0;
	j = 0;
	if (!lptr[i])
		return (bptr);
	while (bptr[i])
	{
		while (bptr[i + j] && bptr[i + j] == lptr[j] && i + j < len)
			if (!lptr[j++ + 1])
				return (&bptr[i]);
		j = 0;
		i++;
	}
	return (0);
}

t_vars	*vars(void)
{
	static t_vars	vars;

	return (&vars);
}
