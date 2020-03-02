/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 15:56:45 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/02 16:01:48 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		display_color_letters(char *c)
{
	int	n;

	if (!c)
		return ;
	n = 0;
	while (n++ < 2)
	{
		if ((*c >= 'a' && *c <= 'h') || (*c >= 'A' && *c <= 'H'))
		{
			if (*c >= 'A' && *c <= 'H')
				ft_putstr("\e[1m");
			ft_putstr("\e[");
			ft_putnbr(30 + *c - ((*c >= 'a' && *c <= 'h') ? 'a' : 'A')
					+ (n == 2 ? 10 : 0));
			ft_putchar('m');
		}
		c++;
	}
}

void		get_env_color(t_ls_entry *ent)
{
	char	*lscolors;
	int		offset;

	lscolors = getenv("LSCOLORS");
	lscolors = lscolors ? lscolors : "Gxfxcxdxbxegedabagacad";
	offset = -1;
	if (S_ISDIR(ent->stat.st_mode))
		offset = 0;
	else if (S_ISLNK(ent->stat.st_mode))
		offset = 1;
	else if (S_ISSOCK(ent->stat.st_mode))
		offset = 2;
	else if (S_ISFIFO(ent->stat.st_mode))
		offset = 3;
	else if (ent->stat.st_mode & S_IXUSR && ent->stat.st_mode & S_ISUID)
		offset = 7;
	else if (ent->stat.st_mode & S_IXUSR && ent->stat.st_mode & S_ISGID)
		offset = 8;
	else if (ent->stat.st_mode & S_IXUSR)
		offset = 4;
	else if (S_ISBLK(ent->stat.st_mode))
		offset = 5;
	else if (S_ISCHR(ent->stat.st_mode))
		offset = 6;
	(void)(offset < 0 ? 0 : display_color_letters(lscolors + 2 * offset));
}

void		display_name(t_ls_opts *opts, t_ls_entry *ent)
{
	if (opts->opts.gg && isatty(1))
		get_env_color(ent);
	ft_putstr(ent->str.name);
	if (opts->opts.gg && isatty(1))
		ft_putstr("\e[0m");
}
