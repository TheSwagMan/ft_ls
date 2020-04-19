/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 18:50:58 by tpotier           #+#    #+#             */
/*   Updated: 2020/04/19 20:09:01 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		add_opt(t_ls_opts *opts, char *sopts)
{
	while (*(++sopts))
	{
		if (!ft_strchr("lRartnuU1G@f", *sopts))
		{
			ft_putstr_fd(opts->name, 2);
			ft_putstr_fd(": illegal option -- '", 2);
			ft_putchar_fd(*sopts, 2);
			ft_putendl_fd("'", 2);
			ft_putendl_fd("usage: ls [-lRartnuU1G@f] [file ...]", 2);
			exit(EXIT_ERR);
		}
		opts->opts.l = *sopts == 'l' ? 1 : opts->opts.l;
		opts->opts.rr = *sopts == 'R' ? 1 : opts->opts.rr;
		opts->opts.a = *sopts == 'a' ? 1 : opts->opts.a;
		opts->opts.r = *sopts == 'r' ? 1 : opts->opts.r;
		opts->opts.t = *sopts == 't' ? 1 : opts->opts.t;
		opts->opts.n = *sopts == 'n' ? 1 : opts->opts.n;
		opts->opts.u = *sopts == 'u' ? 1 : opts->opts.u;
		opts->opts.uu = *sopts == 'U' ? 1 : opts->opts.uu;
		opts->opts.o1 = *sopts == '1' ? 1 : opts->opts.o1;
		opts->opts.gg = *sopts == 'G' ? 1 : opts->opts.gg;
		opts->opts.aaa = *sopts == '@' ? 1 : opts->opts.aaa;
		opts->opts.f = *sopts == 'f' ? 1 : opts->opts.f;
		opts->opts.a = *sopts == 'f' ? 1 : opts->opts.a;
	}
}

void		parse_opts(t_ls_opts *opts, int ac, char **av)
{
	int	k;
	int	n;

	k = paths_count(ac, av);
	opts->dpaths = NULL;
	opts->fpaths = NULL;
	opts->name = av[0];
	opts->opts.n_ = k > 1 ? 1 : opts->opts.n_;
	if (!k)
		lst_append(&opts->dpaths, ft_strdup("."));
	n = 0;
	while (++n < ac)
		if (*(av[n]) == '-')
			add_opt(opts, av[n]);
		else if (k)
			lst_append(is_directory(av[n])
					? &opts->dpaths : &opts->fpaths, ft_strdup(av[n]));
}

void		init_opts(t_opts *opts)
{
	opts->l = 0;
	opts->rr = 0;
	opts->n_ = 0;
	opts->a = 0;
	opts->r = 0;
	opts->t = 0;
	opts->n = 0;
	opts->u = 0;
	opts->uu = 0;
	opts->o1 = 0;
	opts->gg = 0;
}

t_ls_opts	*init_ls_opts(int ac, char **av)
{
	t_ls_opts	*opts;

	if (!(opts = (t_ls_opts *)malloc(sizeof(*opts))))
		ls_exit("malloc failed in init_ls_opts", EXIT_FAT_ERR);
	init_opts(&(opts->opts));
	if (isatty(1))
	{
		opts->opts.gg = 1;
	}
	else
	{
		opts->opts.o1 = 1;
	}
	parse_opts(opts, ac, av);
	if (opts->opts.n)
		opts->opts.l = 1;
	return (opts);
}
