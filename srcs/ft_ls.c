/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:11:30 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/02 16:14:55 by tpotier          ###   ########.fr       */
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
	ls_disp_job(opts, &lst);
	lst_delete(&lst, free_ls_entry);
	return (dirs);
}

void		dir_part(t_ls_opts *opts)
{
	t_lst		*dirs;
	char		*dir;

	while (opts->dpaths)
	{
		dirs = ls(opts->dpaths->data, opts);
		opts->dpaths = opts->dpaths->next;
		if (opts->opts.rr)
		{
			opts->opts.n_ = 1;
			if (dirs)
			{
				lst_goto_n(&dirs, -1);
				while (dirs)
				{
					dir = lst_pop(&dirs);
					if (ft_strcmp(filename(dir), ".")
							&& ft_strcmp(filename(dir), ".."))
						lst_add(&(opts->dpaths), dir);
				}
			}
		}
		if (opts->dpaths)
			ft_putchar('\n');
	}
}

int			main(int ac, char **av)
{
	t_ls_opts	*opts;
	t_lst		*file_data;

	opts = init_ls_opts(ac, av);
	file_data = analyze_path_lst(opts, opts->fpaths);
	ls_disp_job(opts, &file_data);
	if (file_data && opts->dpaths)
		ft_putchar('\n');
	lst_delete(&file_data, free_ls_entry);
	dir_part(opts);
	return (0);
}
