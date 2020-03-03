/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:11:30 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/03 19:14:05 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		free_ls_entry(void *tmp)
{
	t_ls_entry	*ent;

	if (!tmp)
		return ;
	ent = (t_ls_entry *)tmp;
	free(ent->str.mode);
	free(ent->str.nlink);
	free(ent->str.owner);
	free(ent->str.group);
	free(ent->str.size);
	free(ent->str.date);
	free(ent->str.name);
	free(ent->fullpath);
	free(ent);
}

t_lst		*ls(char *path, t_ls_opts *opts, int *ret)
{
	t_lst		*lst;
	t_lst		*dirs;

	lst = NULL;
	if (opts->opts.n_)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	dirs = dir_analyze(opts, path, &lst, ret);
	if (opts->opts.l && lst)
	{
		ft_putstr("total ");
		ft_putnbr(total_dir(lst));
		ft_putchar('\n');
	}
	ls_disp_job(opts, lst);
	lst_delete(&lst, free_ls_entry);
	return (dirs);
}

void		disp_lst(t_lst *lst)
{
	lst_goto_n(&lst, 0);
	while (lst)
	{
		ft_putstr(lst->data);
		ft_putstr(", ");
		lst = lst->next;
	}
	ft_putendl("");
}

void		dir_part(t_ls_opts *opts, int *r)
{
	t_lst		*dirs;
	char		*dir;

	while (opts->dpaths)
	{
		dir = lst_pop(&(opts->dpaths));
		dirs = ls(dir, opts, r);
		free(dir);
		if (opts->opts.rr)
			opts->opts.n_ = 1;
		if (dirs && opts->opts.rr)
			lst_goto_n(&dirs, -1);
		while (dirs && opts->opts.rr)
		{
			dir = lst_pop(&dirs);
			if (ft_strcmp(filename(dir), ".")
					&& ft_strcmp(filename(dir), ".."))
				lst_add(&(opts->dpaths), dir);
			else
				free(dir);
		}
		if (opts->dpaths)
			ft_putchar('\n');
	}
}

int			main(int ac, char **av)
{
	t_ls_opts	*opts;
	t_lst		*file_data;
	int			r;

	r = 0;
	opts = init_ls_opts(ac, av);
	file_data = analyze_path_lst(opts, opts->fpaths, &r);
	lst_delete(&(opts->fpaths), free);
	ls_disp_job(opts, file_data);
	if (file_data && opts->dpaths)
		ft_putchar('\n');
	lst_delete(&file_data, free_ls_entry);
	dir_part(opts, &r);
	free(opts);
	opts = NULL;
	return (r);
}
