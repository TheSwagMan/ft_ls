/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:11:30 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:19:36 by tpotier          ###   ########.fr       */
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

void		ls(char *path, t_ls_opts *opts)
{
	t_lst		*lst;

	lst = NULL;
	if (opts->opts.n_)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	dir_analyze(opts, path, &lst);
	if (opts->opts.l)
	{
		ft_putstr("total ");
		ft_putnbr(total_dir(lst));
		ft_putchar('\n');
	}
	ls_disp_job(opts, lst);
	lst_delete(&lst, free_ls_entry);
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
		ls(opts->dpaths->data, opts);
		if ((opts->dpaths = opts->dpaths->next))
			ft_putchar('\n');
	}
	return (0);
}
