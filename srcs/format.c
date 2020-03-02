/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:10:49 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/02 16:05:40 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		format_perms(mode_t mode, char *res)
{
	res[1] = mode & S_IRUSR ? 'r' : '-';
	res[2] = mode & S_IWUSR ? 'w' : '-';
	res[3] = mode & S_IXUSR ? 'x' : '-';
	if (mode & S_ISUID)
		res[3] = mode & S_IXUSR ? 's' : 'S';
	res[4] = mode & S_IRGRP ? 'r' : '-';
	res[5] = mode & S_IWGRP ? 'w' : '-';
	res[6] = mode & S_IXGRP ? 'x' : '-';
	if (mode & S_ISGID)
		res[6] = mode & S_IXGRP ? 's' : 'S';
	res[7] = mode & S_IROTH ? 'r' : '-';
	res[8] = mode & S_IWOTH ? 'w' : '-';
	res[9] = mode & S_IXOTH ? 'x' : '-';
	if (mode & S_ISVTX)
		res[6] = mode & S_IXOTH ? 't' : 'T';
}

char		*mode_to_str(mode_t mode)
{
	char	*res;

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
	format_perms(mode, res);
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
