/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:09:19 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 22:05:52 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		display_color_letters(char *c)
{
	int	n;

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

	lscolors = ft_strdup(getenv("LSCOLORS"));
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
	if (offset < 0)
		return ;
	display_color_letters(lscolors + 2 * offset);
	free(lscolors);
}

void		display_name(t_ls_opts *opts, t_ls_entry *ent)
{
	if (opts->opts.gg && isatty(1))
		get_env_color(ent);
	ft_putstr(ent->str.name);
	if (opts->opts.gg && isatty(1))
		ft_putstr("\e[0m");
}

void		display_link(t_ls_entry *ent)
{
	int		size;
	char	*name;

	if (S_ISLNK(ent->stat.st_mode))
	{
		size = ent->stat.st_size ? ent->stat.st_size : PATH_MAX;
		if (!(name = malloc(size + 1)))
			ls_exit("Malloc", EXIT_FAT_ERR);
		if (readlink(ent->fullpath, name, size) != ent->stat.st_size
				&& ent->stat.st_size)
			ls_exit("readlink", EXIT_FAT_ERR);
		name[size] = '\0';
		ft_putstr(" -> ");
		ft_putstr(name);
		free(name);
	}
}

void		display_entry_list_long(t_ls_opts *op, t_lst *lst, t_entry_str *max)
{
	t_ls_entry		*ent;

	while (lst)
	{
		ent = lst->data;
		ft_putstr(ent->str.mode);
		ft_putnchar(' ', 2 + (max->nlink_s - ent->str.nlink_s));
		ft_putstr(ent->str.nlink);
		ft_putchar(' ');
		ft_putstr(ent->str.owner);
		ft_putnchar(' ', 2 + (max->owner_s - ent->str.owner_s));
		ft_putstr(ent->str.group);
		ft_putnchar(' ', 1 + (max->group_s - ent->str.group_s));
		ft_putnchar(' ', 1 + (max->size_s - ent->str.size_s));
		ft_putstr(ent->str.size);
		ft_putchar(' ');
		ft_putstr(ent->str.date);
		ft_putchar(' ');
		display_name(op, ent);
		display_link(ent);
		ft_putchar('\n');
		lst = lst->next;
	}
}

void		display_entry_list_short(t_ls_opts *op, t_lst *lst, t_entry_str *max)
{
	t_ls_entry	*ent;
	long		n;
	long		k;
	long		l;
	long		file_count;

	if (!lst)
		return ;
	file_count = lst_size(lst);
	l = -1;
	n = 1 + (max->name_s + 1) * file_count / term_size();
	while (++l < n)
	{
		k = -1;
		while (++k <= file_count / n)
			if (k * n + l < file_count)
			{
				lst_goto_n(&lst, k * n + l);
				ent = ((t_ls_entry *)lst->data);
				display_name(op, ent);
				if ((k + 1) * n + l < file_count)
					ft_putnchar(' ', 1 + (max->name_s - ent->str.name_s));
			}
		ft_putchar('\n');
	}
}

void		display_entry_list_simple(t_ls_opts *op, t_lst *lst)
{
	while (lst)
	{
		display_name(op, lst->data);
		ft_putchar('\n');
		lst = lst->next;
	}
}

void		ls_disp_job(t_ls_opts *opts, t_lst *lst)
{
	t_entry_str	*max;

	if (opts->opts.t)
	{
		if (opts->opts.uu)
			sort_entry_list(opts, &lst, sort_by_cdate);
		else if (opts->opts.u)
			sort_entry_list(opts, &lst, sort_by_adate);
		else
			sort_entry_list(opts, &lst, sort_by_mdate);
	}
	else
		sort_entry_list(opts, &lst, sort_by_name);
	max = get_max_size(lst);
	lst_goto_n(&lst, 0);
	if (opts->opts.l)
		display_entry_list_long(opts, lst, max);
	else if (opts->opts.o1)
		display_entry_list_simple(opts, lst);
	else
		display_entry_list_short(opts, lst, max);
	free(max);
}
