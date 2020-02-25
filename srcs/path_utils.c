/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:12:18 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:12:56 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			is_hidden(char *path)
{
	int	n;

	n = ft_strlen(path);
	while (n && path[n] != '/')
		n--;
	n += n ? 1 : 0;
	return (path[n] == '.' ? 1 : 0);
}

int			paths_count(int ac, char **av)
{
	int	n;

	n = 0;
	while (ac > 1)
		n += (*(av[--ac]) == '-') ? 0 : 1;
	return (n);
}

char		*path_cat(char *dir, char *file)
{
	char	*res;

	if (!(res = malloc(ft_strlen(dir) + ft_strlen(file) + 2)))
		ls_exit("Malloc", EXIT_FAT_ERR);
	*res = '\0';
	ft_strcat(res, dir);
	ft_strcat(res, "/");
	ft_strcat(res, file);
	return (res);
}