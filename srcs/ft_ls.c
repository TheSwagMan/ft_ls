#include "ft_ls.h"

void		ls_exit(char *msg, char code)
{
	perror(msg);
	exit(code);
}

int			paths_count(int ac, char **av)
{
	int	n;

	n = 0;
	while (ac > 1)
		n += (*(av[--ac]) == '-') ? 0 : 1;
	return (n);
}

void		add_opt(t_ls_opts *opts, char *sopts)
{
	while (*(++sopts))
	{
		if (!ft_strchr("lRartnuU1G", *sopts))
		{
			ft_putstr_fd(opts->name, 2);
			ft_putstr_fd(": invalid option -- '", 2);
			ft_putchar_fd(*sopts, 2);
			ft_putendl_fd("'", 2);
			exit(EXIT_FAT_ERR);
		}
		opts->opts.l = *sopts == 'l' ? 1 : opts->opts.l;
		opts->opts.rr = *sopts == 'R' ? 1 : opts->opts.rr;
		opts->opts._n = *sopts == 'R' ? 1 : opts->opts._n;
		opts->opts.a = *sopts == 'a' ? 1 : opts->opts.a;
		opts->opts.r = *sopts == 'r' ? 1 : opts->opts.r;
		opts->opts.t = *sopts == 't' ? 1 : opts->opts.t;
		opts->opts.n = *sopts == 'n' ? 1 : opts->opts.n;
		opts->opts.u = *sopts == 'u' ? 1 : opts->opts.u;
		opts->opts.uu = *sopts == 'U' ? 1 : opts->opts.uu;
		opts->opts._1 = *sopts == '1' ? 1 : opts->opts._1;
		opts->opts.gg = *sopts == 'G' ? 1 : opts->opts.gg;
	}
}

char		is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		perror("Stat");
	return (st.st_mode & S_IFDIR ? 1 : 0);
}

void		parse_opts(t_ls_opts *opts, int ac, char **av)
{
	int	k;
	int	n;

	k = paths_count(ac, av);
	opts->dpaths = NULL;
	opts->fpaths = NULL;
	opts->name = av[0];
	opts->opts._n = k > 1 ? 1 : opts->opts._n;
	if (!k)
		lst_append(&opts->dpaths, ".");
	n = 0;
	while (++n < ac)
		if (*(av[n]) == '-')
			add_opt(opts, av[n]);
		else if (k)
			lst_append(is_directory(av[n]) ? &opts->dpaths : &opts->fpaths, av[n]);
}

void		init_opts(t_opts *opts)
{
	opts->l = 0;
	opts->rr = 0;
	opts->_n = 0;
	opts->a = 0;
	opts->r = 0;
	opts->t = 0;
	opts->n = 0;
	opts->u = 0;
	opts->uu = 0;
	opts->_1 = 0;
	opts->gg = 0;
}

t_ls_opts	*init_ls_opts(int ac, char **av)
{
	t_ls_opts	*opts;

	if (!(opts = (t_ls_opts *)malloc(sizeof(*opts))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	init_opts(&(opts->opts));
	if (isatty(1))
	{
		opts->opts.gg = 1;
	}
	else
	{
		opts->opts._1 = 1;
	}
	parse_opts(opts, ac, av);
	return (opts);
}

void		ls(char *path, t_ls_opts *opts)
{
	int			size;
	t_long_item	**lst;

	(void)lst;
	size = 1;//get_long_list(opts, path, &lst);
	if (opts->opts._n)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	/*
	if (opts->opts.l)
		full_long_display(opts, lst, size);
	else if (opts->opts._1)
		full_short_disp(opts, lst, size);
	else
	*/
		ft_putendl("NOT IMPLEMENTED");
}

void	disp_lst(t_lst *lst)
{
	lst_goto_n(&lst, 0);
	while (lst)
	{
		printf("%s\n", lst->data);
		lst = lst->next;
	}
}

t_ls_entry	*analyze_path(char *path)
{
	t_ls_entry	*ent;

	if (!(ent = malloc(sizeof(*ent))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	if (stat(path, &ent->stat) != 0)
		perror("Stat");
	ent->name = path;
	return (ent);
}

void		display_entry_list(t_lst *lst)
{
	t_ls_entry	*ent;

	lst_goto_n(&lst, 0);
	while (lst)
	{
		ent = lst->data;
		ft_printf("%o\t%s\n", ent->stat.st_mode, ent->name);
		lst = lst->next;
	}
}

t_lst		*analyze_path_lst(t_lst *lst)
{
	t_lst	*res;

	res = NULL;
	lst_goto_n(&lst, 0);
	while (lst)
	{
		lst_append(&res, analyze_path(lst->data));
		lst = lst->next;
	}
	return (res);
}

int			main(int ac, char **av)
{
	t_ls_opts	*opts;
	t_lst		*tst;


	opts = init_ls_opts(ac, av);
	tst = analyze_path_lst(opts->fpaths);
	display_entry_list(tst);
	while (opts->dpaths)
	{
		ls(opts->dpaths->data, opts);
		opts->dpaths = opts->dpaths->next;
	}
	return (0);
}
