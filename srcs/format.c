/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:10:49 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:11:22 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char		*mode_to_str(mode_t mode)
{
	char	*res;
	int		i;

	if (!(res = malloc(11)))
		return (NULL);
	res[10] = 0;
	res[0] = S_ISREG(mode) ? '-' : res[0];
	res[0] = S_ISDIR(mode) ? 'd' : res[0];
	res[0] = S_ISLNK(mode) ? 'l' : res[0];
	res[0] = S_ISFIFO(mode) ? 'p' : res[0];
	res[0] = S_ISSOCK(mode) ? 's' : res[0];
	res[0] = S_ISBLK(mode) ? 'b' : res[0];
	res[0] = S_ISCHR(mode) ? 'c' : res[0];
	i = -1;
	while (++i < 3)
	{
		res[1 + 3 * (2 - i)] = ((mode >> (2 + 3 * i)) & 1) ? 'r' : '-';
		res[2 + 3 * (2 - i)] = ((mode >> (1 + 3 * i)) & 1) ? 'w' : '-';
		res[3 + 3 * (2 - i)] = ((mode >> (3 * i)) & 1) ? 'x' : '-';
	}
	return (res);
}

char		*owner_to_str(uid_t uid)
{
	struct passwd	*pw;

	pw = getpwuid(uid);
	return (ft_strdup(pw->pw_name));
}

char		*group_to_str(gid_t gid)
{
	struct group	*gr;

	gr = getgrgid(gid);
	return (ft_strdup(gr->gr_name));
}

char		*date_to_str(time_t tm)
{
	int		i;
	char	*tmp;
	char	date[13];
	time_t	now;

	time(&now);
	tmp = ctime(&tm);
	i = -1;
	while (++i < 7)
		date[i] = tmp[i + 4];
	i = -1;
	while (++i < 5)
		if (tm < now - SEC_IN_6_MON || tm > now)
			date[i + 7] = tmp[i + 19];
		else
			date[i + 7] = tmp[i + 11];
	date[12] = 0;
	return (ft_strdup(date));
}

char		*format_majmin(dev_t rdev)
{
	char	*min;
	char	*maj;
	char	*res;
	uint8_t	i;

	if (!(res = ft_memalloc(8)))
		ls_exit("malloc", EXIT_FAT_ERR);
	i = 0;
	maj = ft_itoa(rdev >> 24 & 0xff);
	min = ft_itoa(rdev & 0xff);
	while (i < 3 - ft_strlen(maj))
		res[i++] = ' ';
	ft_strcat(res, maj);
	ft_strcat(res, ", ");
	i = 0;
	while (i < 3 - ft_strlen(min))
		res[5 + i++] = ' ';
	ft_strcat(res, min);
	free(maj);
	free(min);
	return (res);
}
