#include "ft_ls.h"

int			is_hidden(char *path)
{
	int	n;

	n = ft_strlen(path);
	while (n && path[n] != '/')
		n--;
	n += n ? 1 : 0;
	return (path[n] == '.' ? 1 : 0);
}

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
	long	change;

	while (*(++sopts))
	{
		change = 0;
		change |= *sopts == 'l' ? OPT_L : 0;
		change |= *sopts == 'R' ? OPT_RR : 0;
		change |= *sopts == 'R' ? OPT__N : 0;
		change |= *sopts == 'a' ? OPT_A : 0;
		change |= *sopts == 'r' ? OPT_R : 0;
		change |= *sopts == 't' ? OPT_T : 0;
		change |= *sopts == 'n' ? OPT_N : 0;
		change |= *sopts == '1' ? OPT_1 : 0;
		if (!change)
		{
			ft_putstr_fd(opts->name, 2);
			ft_putstr_fd(": invalid option -- '", 2);
			ft_putchar_fd(*sopts, 2);
			ft_putendl_fd("'", 2);
			exit(EXIT_FAT_ERR);
		}
		opts->opts |= change;
	}
}

void		parse_opts(t_ls_opts *opts, int ac, char **av)
{
	int	k;
	int	n;

	k = paths_count(ac, av);
	if (!(opts->paths = (char **)malloc(((k ? k : 1) + 1) * sizeof(char *))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	opts->paths[k + 1] = NULL;
	opts->name = av[0];
	opts->opts |= k > 1 ? OPT__N : 0;
	if (!k)
		opts->paths[0] = ".";
	n = 0;
	while (++n < ac)
		if (*(av[n]) == '-')
			add_opt(opts, av[n]);
		else if (k)
			opts->paths[--k] = av[n];
}

t_ls_opts	*init_opts(int ac, char **av)
{
	t_ls_opts	*opts;

	if (!(opts = (t_ls_opts *)malloc(sizeof(*opts))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	opts->opts = 0;
	opts->paths = NULL;
	parse_opts(opts, ac, av);
	return (opts);
}

char		*ft_strcat_mal(char *s1, char *s2)
{
	char	*res;
	size_t	size_s1;

	size_s1 = ft_strlen(s1);
	if (!(res = (char *)malloc((size_s1 + ft_strlen(s2) + 1)
					* sizeof(char))))
		return (NULL);
	ft_strcpy(res, s1);
	ft_strcpy(res + size_s1, s2);
	return (res);
}

char		*ft_strcat_mal_free(char *s1, char *s2)
{
	char *res;

	res = ft_strcat_mal(s1, s2);
	free(s1);
	return (res);
}

void		long_disp(t_ls_opts *opts, t_long_item *item)
{
	struct passwd	*upwd;
	struct group	*ggrp;
	char			*stm;
	int				i;

	if (S_ISREG(item->st->st_mode))
		putchar('-');
	if (S_ISDIR(item->st->st_mode))
		putchar('d');
	if (S_ISLNK(item->st->st_mode))
		putchar('l');
	if (S_ISFIFO(item->st->st_mode))
		putchar('p');
	if (S_ISSOCK(item->st->st_mode))
		putchar('s');
	if (S_ISBLK(item->st->st_mode))
		putchar('b');
	if (S_ISCHR(item->st->st_mode))
		putchar('c');
	i = 2;
	while (i >= 0)
	{
		putchar(((item->st->st_mode >> (2 + 3 * i)) & 1) ? 'r' : '-');
		putchar(((item->st->st_mode >> (1 + 3 * i)) & 1) ? 'w' : '-');
		putchar(((item->st->st_mode >> (3 * i)) & 1) ? 'x' : '-');
		i--;
	}
	putchar(' ');
	printf("%ld", item->st->st_nlink);
	putchar(' ');
	upwd = getpwuid(item->st->st_uid);
	ggrp = getgrgid(item->st->st_gid);
	if (upwd && !(opts->opts & OPT_N))
		printf("%s", upwd->pw_name);
	else
		printf("%d", item->st->st_uid);
	putchar(' ');
	if (ggrp && !(opts->opts & OPT_N))
		printf("%s", ggrp->gr_name);
	else
		printf("%d", item->st->st_gid);
	putchar(' ');
	printf("%ld", item->st->st_size);
	stm = ctime(&item->st->st_mtime);
	while (*(stm++) != ' ');
	i = 0;
	while (stm[i++]);
	while (stm[--i] != ':');
	stm[i] = '\0';
	putchar(' ');
	printf("%s", stm);
	putchar(' ');
	printf("%s", item->name);
	putchar('\n');
}

DIR			*get_dir(t_ls_opts *opts, char *path)
{
	DIR				*d;

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

int			dir_count(t_ls_opts *opts, char *path)
{
	DIR				*d;
	struct dirent	*ent;
	int				count;

	count = 0;
	d = get_dir(opts, path);
	while ((ent = readdir(d)))
		if (!is_hidden(ent->d_name) || opts->opts & OPT_A)
			count++;
	(void)closedir(d);
	return (count);
}

int			get_long_list(t_ls_opts *opts, char *path, t_long_item ***lst)
{
	DIR				*d;
	struct dirent	*ent;
	int				size;
	char			*full_path;
	int				n;

	size = dir_count(opts, path);
	*lst = (t_long_item **)malloc(size * sizeof(**lst));
	d = get_dir(opts, path);
	n = 0;
	while ((ent = readdir(d)))
		if (!is_hidden(ent->d_name) || opts->opts & OPT_A)
		{
			if (!((*lst)[n] = (t_long_item *)malloc(sizeof(t_long_item))))
				ls_exit("Malloc error", EXIT_FAT_ERR);
			if (!((*lst)[n]->st
						= (struct stat *)malloc(sizeof(struct stat))))
				ls_exit("Malloc error", EXIT_FAT_ERR);
			full_path = ft_strcat_mal(path, "/");
			full_path = ft_strcat_mal_free(full_path, ent->d_name);
			if (lstat(full_path, (*lst)[n]->st) == -1)
				ls_exit("Stat error", EXIT_FAT_ERR);
			free(full_path);
			(*lst)[n]->name = ft_strdup(ent->d_name);
			(*lst)[n]->short_name = (*lst)[n]->name;
			while (*((*lst)[n]->short_name) == '.')
				((*lst)[n]->short_name)++;
			(*lst)[n]->path = ft_strdup(path);
			n++;
		}
	(void)closedir(d);
	quicksort(opts, *lst, size);
	return (size);
}

void		ls(char *path, t_ls_opts *opts)
{
	int			size;
	int			k;
	t_long_item	**lst;
	int			index;

	size = get_long_list(opts, path, &lst);
	if (opts->opts & OPT__N)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	k = 0;
	while (k < size)
	{
		index = (opts->opts & OPT_R) ? size - k - 1 : k;
		if (opts->opts & OPT_L)
			long_disp(opts, lst[index]);
		else if (opts->opts & OPT_1)
			ft_putendl(lst[index]->name);
		else
		{
			ft_putendl("NOT IMPLEMENTED");
			break ;
		}
		k++;
	}
}

int	main(int ac, char **av)
{
	t_ls_opts	*opts;

	opts = init_opts(ac, av);
	while (*opts->paths)
		ls(*(opts->paths++), opts);
	return (0);
}
