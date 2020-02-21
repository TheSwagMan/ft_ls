#include "ft_ls.h"

t_ls_entry	*analyze_path(t_ls_opts *opts, char *path, char *filename)
{
	t_ls_entry	*ent;

	if (!(ent = malloc(sizeof(*ent))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	ent->fullpath = path_cat(path, filename);
	if (lstat(ent->fullpath, &ent->stat) != 0)
	{
		ft_putstr_fd(opts->name, 2);
		ft_putstr_fd(": cannot access '", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd("': ", 2);
		perror("");
		free(ent->fullpath);
		free(ent);
		return (NULL);
	}
	ent->str.mode = mode_to_str(ent->stat.st_mode);
	ent->str.nlink = ft_itoa(ent->stat.st_nlink);
	ent->str.owner = owner_to_str(ent->stat.st_uid);
	ent->str.group = group_to_str(ent->stat.st_gid);
	if (S_ISCHR(ent->stat.st_mode) || S_ISBLK(ent->stat.st_mode))
		ent->str.size = format_majmin(ent->stat.st_rdev);
	else
		ent->str.size = ft_itoa(ent->stat.st_size);
	ent->str.date = date_to_str(ent->stat.st_mtime);
	ent->str.mode_s = 10;
	ent->str.nlink_s = ft_strlen(ent->str.nlink);
	ent->str.owner_s = ft_strlen(ent->str.owner);
	ent->str.group_s = ft_strlen(ent->str.group);
	ent->str.size_s = ft_strlen(ent->str.size);
	ent->str.date_s = 12;
	ent->name = ft_strdup(filename);
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
			lst_append(flst, analyze_path(opts, path, ent->d_name));
			count++;
		}
	(void)closedir(d);
	return (count);
}

char		is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

t_entry_str	*get_max_size(t_lst *lst)
{
	t_entry_str	*max;
	t_entry_str	tmp;

	if (!(max = malloc(sizeof(*max))))
		ls_exit("Malloc", EXIT_FAT_ERR);
	max->mode_s = 0;
	max->nlink_s = 0;
	max->owner_s = 0;
	max->group_s = 0;
	max->size_s = 0;
	max->date_s = 0;
	lst_goto_n(&lst, 0);
	while (lst)
	{
		tmp = ((t_ls_entry *)lst->data)->str;
		if (tmp.mode_s > max->mode_s)
			max->mode_s = tmp.mode_s;
		if (tmp.nlink_s > max->nlink_s)
			max->nlink_s = tmp.nlink_s;
		if (tmp.owner_s > max->owner_s)
			max->owner_s = tmp.owner_s;
		if (tmp.group_s > max->group_s)
			max->group_s = tmp.group_s;
		if (tmp.size_s > max->size_s)
			max->size_s = tmp.size_s;
		if (tmp.date_s > max->date_s)
			max->date_s = tmp.date_s;
		lst = lst->next;
	}
	return (max);
}

int			total_dir(t_lst *lst)
{
	int	tot;

	tot = 0;
	lst_goto_n(&lst, 0);
	while (lst)
	{
		tot += ((t_ls_entry *)lst->data)->stat.st_blocks;
		lst = lst->next;
	}
	return (tot);
}

t_lst		*analyze_path_lst(t_ls_opts *opts, t_lst *lst)
{
	t_lst		*res;
	t_ls_entry	*tmp;

	res = NULL;
	lst_goto_n(&lst, 0);
	while (lst)
	{
		tmp = analyze_path(opts, ".", lst->data);
		if (tmp)
			lst_append(&res, tmp);
		lst = lst->next;
	}
	return (res);
}

