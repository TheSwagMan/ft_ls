/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:09:19 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:50:22 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

void		display_entry_list_long(t_lst *lst, t_entry_str *max)
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
		ft_putstr(ent->str.name);
		display_link(ent);
		ft_putchar('\n');
		lst = lst->next;
	}
}

size_t		term_size(void)
{
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return (w.ws_col);
}

void		display_entry_list_short(t_lst *lst, t_entry_str *max)
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
				ft_putstr(ent->str.name);
				if ((k + 1) * n + l < file_count)
					ft_putnchar(' ', 1 + (max->name_s - ent->str.name_s));
			}
		ft_putchar('\n');
	}
}

void		display_entry_list_simple(t_lst *lst)
{
	while (lst)
	{
		ft_putstr(((t_ls_entry *)lst->data)->str.name);
		ft_putchar('\n');
		lst = lst->next;
	}
}

void		ls_disp_job(t_ls_opts *opts, t_lst *lst)
{
	t_entry_str	*max;

	if (opts->opts.t)
		sort_entry_list(opts, &lst, sort_by_date);
	else
		sort_entry_list(opts, &lst, sort_by_name);
	max = get_max_size(lst);
	lst_goto_n(&lst, 0);
	if (opts->opts.l)
		display_entry_list_long(lst, max);
	else if (opts->opts.o1)
		display_entry_list_simple(lst);
	else
		display_entry_list_short(lst, max);
	free(max);
}
