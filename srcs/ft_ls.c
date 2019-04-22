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

void		ft_putpadl(char *str, size_t size, size_t pad, char pad_char)
{
	size_t	i;

	i = 0;
	while (i++ < pad - size)
		ft_putchar(pad_char);
	ft_putstr(str);
}

void		ft_putpadr(char *str, size_t size, size_t pad, char pad_char)
{
	size_t	i;

	i = 0;
	ft_putstr(str);
	while (i++ < pad - size)
		ft_putchar(pad_char);
}

t_long_item	*get_item(t_ls_opts *opts, t_long_item **items, int size, int k)
{
	return (items[(opts->opts & OPT_R) ? size - k - 1 : k]);
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
		change |= *sopts == 'G' ? OPT_GG : 0;
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
	opts->paths[k ? k : 1] = NULL;
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
	if (isatty(1))
	{
		opts->opts |= OPT_GG;
		opts->opts |= OPT_1;
	}
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

void		long_to_str(t_ls_opts *opts, t_long_item *item)
{
	int				i;
	struct passwd	*upwd;
	struct group	*ggrp;

	if (!(item->par = (t_itm_parsd *)malloc(sizeof(t_itm_parsd))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	if (!(item->par->rules = (char *)malloc(11 * sizeof(char))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	item->par->rules[10] = 0;
	item->par->rules[0] = S_ISREG(item->st->st_mode) ? '-' : item->par->rules[0];
	item->par->rules[0] = S_ISDIR(item->st->st_mode) ? 'd' : item->par->rules[0];
	item->par->rules[0] = S_ISLNK(item->st->st_mode) ? 'l' : item->par->rules[0];
	item->par->rules[0] = S_ISFIFO(item->st->st_mode) ? 'p' : item->par->rules[0];
	item->par->rules[0] = S_ISSOCK(item->st->st_mode) ? 's' : item->par->rules[0];
	item->par->rules[0] = S_ISBLK(item->st->st_mode) ? 'b' : item->par->rules[0];
	item->par->rules[0] = S_ISCHR(item->st->st_mode) ? 'c' : item->par->rules[0];
	i = -1;
	while (++i < 3)
	{
		item->par->rules[1 + 3 * (2 - i)] = ((item->st->st_mode >> (2 + 3 * i)) & 1) ? 'r' : '-';
		item->par->rules[2 + 3 * (2 - i)] = ((item->st->st_mode >> (1 + 3 * i)) & 1) ? 'w' : '-';
		item->par->rules[3 + 3 * (2 - i)] = ((item->st->st_mode >> (3 * i)) & 1) ? 'x' : '-';
	}
	if (!(item->par->nlink = ft_itoa(item->st->st_nlink)))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	upwd = getpwuid(item->st->st_uid);
	ggrp = getgrgid(item->st->st_gid);
	if (upwd && !(opts->opts & OPT_N))
		item->par->owner = upwd->pw_name;
	else
		if (!(item->par->owner = ft_itoa(item->st->st_uid)))
			ls_exit("Malloc error", EXIT_FAT_ERR);
	if (ggrp && !(opts->opts & OPT_N))
		item->par->group = ggrp->gr_name;
	else
		if (!(item->par->group = ft_itoa(item->st->st_gid)))
			ls_exit("Malloc error", EXIT_FAT_ERR);
	if (!(item->par->size = ft_itoa(item->st->st_size)))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	item->par->date = ctime(&item->st->st_mtime);
	while (*(item->par->date++) != ' ');
	i = 0;
	while (item->par->date[i++]);
	while (item->par->date[--i] != ':');
	item->par->date[i] = '\0';
	if (!(item->par->date = ft_strdup(item->par->date)))
		ls_exit("Malloc error", EXIT_FAT_ERR);
}

void		set_parsed_size(t_long_item *item)
{
	item->par->nlink_s = ft_strlen(item->par->nlink);
	item->par->owner_s = ft_strlen(item->par->owner);
	item->par->group_s = ft_strlen(item->par->group);
	item->par->size_s = ft_strlen(item->par->size);
}

void		short_display(t_ls_opts *opts, t_long_item *item)
{
	if (opts->opts & OPT_GG)
	{
		if (item->par->rules[0] == 'd')
			ft_putstr("\x1B[1;96m");
		else if (item->par->rules[0] == 'l')
			ft_putstr("\x1B[35m");
		else if (item->par->rules[3] == 'x')
			ft_putstr("\x1B[31m");
	}
	ft_putstr(item->name);
	if (opts->opts & OPT_GG)
		ft_putstr("\x1B[0m");
	if (item->par->rules[0] == 'l' && opts->opts & OPT_L)
	{
		ft_putstr(" -> ");
		ft_putstr(item->link_name);
	}
	if (opts->opts & OPT_1 || opts->opts & OPT_L)
		ft_putchar('\n');
}

void		long_disp(t_ls_opts *opts, t_long_item *item, t_itm_parsd *max)
{
	ft_putstr(item->par->rules);
	ft_putstr("  ");
	ft_putpadl(item->par->nlink, item->par->nlink_s, max->nlink_s, ' ');
	ft_putchar(' ');
	ft_putpadr(item->par->owner, item->par->owner_s, max->owner_s, ' ');
	ft_putstr("  ");
	ft_putpadr(item->par->group, item->par->group_s, max->group_s, ' ');
	ft_putstr("  ");
	ft_putpadl(item->par->size, item->par->size_s, max->size_s, ' ');
	ft_putchar(' ');
	ft_putstr(item->par->date);
	ft_putchar(' ');
	short_display(opts, item);
}

t_itm_parsd	*get_max(t_ls_opts *opts, t_long_item **items, int size)
{
	t_itm_parsd	*max;
	int			k;

	max = (t_itm_parsd *)malloc(sizeof(*max));
	k = 0;
	max->nlink_s = 0;
	max->owner_s = 0;
	max->group_s = 0;
	max->size_s = 0;
	while (k < size)
	{
		long_to_str(opts, items[k]);
		set_parsed_size(items[k]);
		if (items[k]->par->nlink_s > max->nlink_s)
			max->nlink_s = items[k]->par->nlink_s;
		if (items[k]->par->owner_s > max->owner_s)
			max->owner_s = items[k]->par->owner_s;
		if (items[k]->par->group_s > max->group_s)
			max->group_s = items[k]->par->group_s;
		if (items[k]->par->size_s > max->size_s)
			max->size_s = items[k]->par->size_s;
		k++;
	}
	return (max);
}

void		full_long_display(t_ls_opts *opts, t_long_item **lst, int size)
{
	int			k;
	t_itm_parsd	*max;
	int			bcount;

	max = get_max(opts, lst, size);
	bcount = 0;
	k = 0;
	while (k < size)
		bcount += lst[k++]->st->st_blocks;
	ft_putstr("total ");
	ft_putnbr(bcount);
	ft_putchar('\n');
	k = 0;
	while (k < size)
		long_disp(opts, get_item(opts, lst, size, k++), max);
	free(max);
}

void		full_short_disp(t_ls_opts *opts, t_long_item **lst, int size)
{
	int			k;

	k = 0;
	while (k < size)
		short_display(opts, get_item(opts, lst, size, k++));
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
			if (S_ISLNK((*lst)[n]->st->st_mode))
			{
				if (!((*lst)[n]->link_name =
							(char *)ft_memalloc((FNAME_SIZE + 1) * sizeof(char))))
					ls_exit("Malloc error", EXIT_FAT_ERR);
				readlink(full_path, (*lst)[n]->link_name, FNAME_SIZE);
			}
			free(full_path);
			(*lst)[n]->name = ft_strdup(ent->d_name);
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
	t_long_item	**lst;

	size = get_long_list(opts, path, &lst);
	if (opts->opts & OPT__N)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	if (opts->opts & OPT_L)
		full_long_display(opts, lst, size);
	else if (opts->opts & OPT_1)
		full_short_disp(opts, lst, size);
	else
		ft_putendl("NOT IMPLEMENTED");
}

int	main(int ac, char **av)
{
	t_ls_opts	*opts;

	opts = init_opts(ac, av);
	while (*opts->paths)
		ls(*(opts->paths++), opts);
	return (0);
}
