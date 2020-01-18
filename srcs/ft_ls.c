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

void		parse_opts(t_ls_opts *opts, int ac, char **av)
{
	int	k;
	int	n;

	k = paths_count(ac, av);
	if (!(opts->paths = (char **)malloc(((k ? k : 1) + 1) * sizeof(char *))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	opts->paths[k ? k : 1] = NULL;
	opts->name = av[0];
	opts->opts._n = k > 1 ? 1 : opts->opts._n;
	if (!k)
		opts->paths[0] = ".";
	n = 0;
	while (++n < ac)
		if (*(av[n]) == '-')
			add_opt(opts, av[n]);
		else if (k)
			opts->paths[--k] = av[n];
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
	opts->paths = NULL;
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

char		is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		perror("Stat");
	return (st.st_mode & S_IFDIR ? 1 : 0);
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

int			main(int ac, char **av)
{
	t_ls_opts	*opts;
	int			i;

	opts = init_ls_opts(ac, av);
	/*printf("l: %d\n", opts->opts.l);
	printf("rr: %d\n", opts->opts.rr);
	printf("_n: %d\n", opts->opts._n);
	printf("a: %d\n", opts->opts.a);
	printf("r: %d\n", opts->opts.r);
	printf("t: %d\n", opts->opts.t);
	printf("n: %d\n", opts->opts.n);
	printf("u: %d\n", opts->opts.u);
	printf("uu: %d\n", opts->opts.uu);
	printf("_1: %d\n", opts->opts._1);
	printf("gg: %d\n", opts->opts.gg);
	*/
	i = 0;
	while (opts->paths[i])
	while (*opts->paths)
	{
		printf("%s -> %d\n", *opts->paths, is_directory(*opts->paths));
		ls(*(opts->paths++), opts);
	}
	return (0);
}
