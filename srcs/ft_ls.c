#include "ft_ls.h"

void		ls_exit(char *msg, char code)
{
	if (errno)
		perror(msg);
	else
		ft_putstr_fd(msg, STDERR_FILENO);
	exit(code);
}

int			is_hidden(char *path)
{
	int	n;

	n = ft_strlen(path);
	while (n && path[n] != '/')
		n--;
	n += n ? 1 : 0;
	return (path[n] == '.' ? 1 : 0);
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

char		*mode_to_str(mode_t mode)
{
	char	*res;
	int		i;

	if (!(res = malloc(11)))
		return (NULL);
	res[10] = 0;
	res[0] = S_ISREG(mode) ? '-' : res[0];
	res[0] = S_ISDIR(mode) ? 'd' : res[0];
	res[0] = S_ISLNK(mode) ? 'l' : res[0];
	res[0] = S_ISFIFO(mode) ? 'p' : res[0];
	res[0] = S_ISSOCK(mode) ? 's' : res[0];
	res[0] = S_ISBLK(mode) ? 'b' : res[0];
	res[0] = S_ISCHR(mode) ? 'c' : res[0];
	i = -1;
	while (++i < 3)
	{
		res[1 + 3 * (2 - i)] = ((mode >> (2 + 3 * i)) & 1) ? 'r' : '-';
		res[2 + 3 * (2 - i)] = ((mode >> (1 + 3 * i)) & 1) ? 'w' : '-';
		res[3 + 3 * (2 - i)] = ((mode >> (3 * i)) & 1) ? 'x' : '-';
	}
	return (res);
}

char		*owner_to_str(uid_t uid)
{
	struct passwd	*pw;
	
	pw = getpwuid(uid);
	return (pw->pw_name);
}

char		*group_to_str(gid_t gid)
{
	struct group	*gr;
	
	gr = getgrgid(gid);
	return (gr->gr_name);
}

char		*date_to_str(time_t tm)
{
	int		i;
	char	*date;

	// If the modification time of the file is more than 6 months in the past or future, then the year of the last modification is displayed in place of the hour and minute fields.
	date = ctime(&tm);
	while (*(date++) != ' ');
	i = 0;
	while (date[i++]);
	while (date[--i] != ':');
	date[i] = '\0';
	return (date);
}
t_ls_entry	*analyze_path(char *path)
{
	t_ls_entry	*ent;

	if (!(ent = malloc(sizeof(*ent))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	if (lstat(path, &ent->stat) != 0)
		perror("lstat");
	ent->str.mode = mode_to_str(ent->stat.st_mode);
	ent->str.nlink = ft_itoa(ent->stat.st_nlink);
	ent->str.owner = owner_to_str(ent->stat.st_uid);
	ent->str.group = group_to_str(ent->stat.st_gid);
	ent->str.size = ft_itoa(ent->stat.st_size);
	/*
	if (S_ISLNK(ent->stat.st_mode) && stat(path, &ent->rstat) != 0)
	{
		ft_putendl(path);
		perror("stat");
	}
	*/
	ent->name = path;

	return (ent);
}

DIR			*get_dir(t_ls_opts *opts, char *path)
{
	DIR	*d;

	if (!(d = opendir(path)))
	{
		ft_putstr_fd(opts->name, 2);
		ft_putstr_fd(": cannot access '", 2);
		ft_putstr_fd(path, 2);
		perror("'");
		exit(EXIT_FAT_ERR);
	}
	return (d);
}

char		*path_cat(char *dir, char *file)
{
	char	*res;

	if (!(res = malloc(ft_strlen(dir) + ft_strlen(file) + 2)))
		ls_exit("Malloc", EXIT_FAT_ERR);
	*res = '\0';
	ft_strcat(res, dir);
	ft_strcat(res, "/");
	ft_strcat(res, file);
	return (res);
}

int			dir_analyze(t_ls_opts *opts, char *path, t_lst **flst)
{
	DIR				*d;
	struct dirent	*ent;
	int				count;

	count = 0;
	d = get_dir(opts, path);
	while ((ent = readdir(d)))
		if (!is_hidden(ent->d_name) || opts->opts.a)
		{
			lst_append(flst, analyze_path(path_cat(path, ent->d_name)));
			count++;
		}
	(void)closedir(d);
	return (count);
}

char		is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		perror("Stat");
	return (S_ISDIR(st.st_mode));
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

void	disp_lst(t_lst *lst)
{
	lst_goto_n(&lst, 0);
	while (lst)
	{
		printf("%s\t", lst->data);
		lst = lst->next;
	}
}

void		display_entry_list(t_lst *lst)
{
	t_ls_entry		*ent;
	struct group	*gr;
	char			*name;

	lst_goto_n(&lst, 0);
	while (lst)
	{
		ent = lst->data;
		ft_putstr(ent->str.mode);
		ft_putchar(' ');
		ft_putstr(ent->str.nlink);
		ft_putchar(' ');
		ft_putstr(ent->str.owner);
		ft_putchar(' ');
		ft_putstr(ent->str.group);
		ft_putchar(' ');
		ft_putstr(ent->str.size);
		ft_putchar(' ');
		ft_putstr(ent->str.date);
		ft_putchar(' ');
		ft_putstr(ent->name);
		gr = getgrgid(ent->stat.st_gid);
		if (S_ISLNK(ent->stat.st_mode))
		{
			if (!(name = malloc(ent->stat.st_size + 1)))
				ls_exit("Malloc", EXIT_FAT_ERR);
			if (readlink(ent->name, name, ent->stat.st_size + 1) != ent->stat.st_size)
				ls_exit("readlink", EXIT_FAT_ERR);
			name[ent->stat.st_size] = '\0';
			ft_printf(" -> %s", name);
		}
		ft_putchar('\n');
		lst = lst->next;
	}
}

int			sort_by_name(void *e1, void *e2)
{
	return (ft_strcmp(((t_ls_entry *)e1)->name, ((t_ls_entry *)e2)->name) > 0);
}

void		sort_entry_list(t_lst **lst, int (*f)(void *e1, void *e2))
{
	t_lst	*new;

	if (!lst || !*lst)
		return ;
	new = NULL;
	lst_goto_n(lst, 0);
	while (*lst)
		lst_insert_sorted(&new, lst_pop(lst), f);
	*lst = new;
}

void		ls(char *path, t_ls_opts *opts)
{
	int		size;
	t_lst	*lst;

	lst = NULL;
	size = 1;//get_long_list(opts, path, &lst);
	if (opts->opts._n)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	dir_analyze(opts, path, &lst);
	sort_entry_list(&lst, sort_by_name);
	display_entry_list(lst);
	/*
	if (opts->opts.l)
		full_long_display(opts, lst, size);
	else if (opts->opts._1)
		full_short_disp(opts, lst, size);
	else
	*/
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
	sort_entry_list(&tst, sort_by_name);
	display_entry_list(tst);
	if (tst)
		ft_putchar('\n');
	while (opts->dpaths)
	{
		ls(opts->dpaths->data, opts);
		if ((opts->dpaths = opts->dpaths->next))
			ft_putchar('\n');
	}
	return (0);
}
