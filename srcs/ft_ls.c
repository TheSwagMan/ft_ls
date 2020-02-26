/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:11:30 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 22:55:15 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ls_exit(char *msg, char code)
{
	if (errno)
		perror(msg);
	else
		ft_putstr_fd(msg, STDERR_FILENO);
	exit(code);
}

void		free_ls_entry(void *tmp)
{
	t_ls_entry	*ent;

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

t_lst		*ls(char *path, t_ls_opts *opts)
{
	t_lst		*lst;
	t_lst		*dirs;

	lst = NULL;
	if (opts->opts.n_)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	dirs = dir_analyze(opts, path, &lst);
	if (opts->opts.l)
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
	while (lst)
	{
		ft_putstr("- ");
		ft_putendl(lst->data);
		lst = lst->next;
	}
}

int			main(int ac, char **av)
{
	t_ls_opts	*opts;
	t_lst		*tst;

	opts = init_ls_opts(ac, av);
	tst = analyze_path_lst(opts, opts->fpaths);
	ls_disp_job(opts, tst);
	if (tst && opts->dpaths)
		ft_putchar('\n');
	lst_delete(&tst, free_ls_entry);
	while (opts->dpaths)
	{
		tst = ls(opts->dpaths->data, opts);
		opts->opts.n_ = 1;
		opts->dpaths = opts->dpaths->next;
		if (tst)
		{
			lst_goto_n(&tst, -1);
			while (tst->prev)
			{
				lst_add(&(opts->dpaths), ft_strdup(tst->data));
				tst = tst->prev;
			}
			lst_add(&(opts->dpaths), ft_strdup(tst->data));
			lst_delete(&tst, free);
		}
		if (opts->dpaths)
			ft_putchar('\n');
	}
	return (0);
}
