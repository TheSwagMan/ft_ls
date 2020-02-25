/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 18:51:28 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 22:53:32 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		set_fields(t_ls_opts *opts, t_ls_entry *ent)
{
	ent->str.mode = mode_to_str(ent->stat.st_mode);
	ent->str.nlink = ft_itoa(ent->stat.st_nlink);
	ent->str.owner = opts->opts.n ? ft_itoa(ent->stat.st_uid)
		: owner_to_str(ent->stat.st_uid);
	ent->str.group = opts->opts.n ? ft_itoa(ent->stat.st_gid)
		: group_to_str(ent->stat.st_gid);
	if (S_ISCHR(ent->stat.st_mode) || S_ISBLK(ent->stat.st_mode))
		ent->str.size = format_majmin(ent->stat.st_rdev);
	else
		ent->str.size = ft_itoa(ent->stat.st_size);
	if (opts->opts.uu)
		ent->str.date = date_to_str(ent->stat.st_ctime);
	else if (opts->opts.u)
		ent->str.date = date_to_str(ent->stat.st_atime);
	else
		ent->str.date = date_to_str(ent->stat.st_mtime);
	ent->str.mode_s = 10;
	ent->str.nlink_s = ft_strlen(ent->str.nlink);
	ent->str.owner_s = ft_strlen(ent->str.owner);
	ent->str.group_s = ft_strlen(ent->str.group);
	ent->str.size_s = ft_strlen(ent->str.size);
	ent->str.date_s = 12;
}

void		init_ls_entry(t_ls_entry *ent)
{
	ent->fullpath = NULL;
	ent->str.name = NULL;
	ent->str.mode = NULL;
	ent->str.nlink = NULL;
	ent->str.owner = NULL;
	ent->str.group = NULL;
	ent->str.size = NULL;
	ent->str.date = NULL;
}

t_ls_entry	*analyze_path(t_ls_opts *opts, char *path, char *filename)
{
	t_ls_entry	*ent;

	if (!(ent = malloc(sizeof(*ent))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	init_ls_entry(ent);
	ent->fullpath = path_cat(path, filename);
	ent->str.name = ft_strdup(filename);
	ent->str.name_s = ft_strlen(ent->str.name);
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
	if (opts->opts.l)
		set_fields(opts, ent);
	return (ent);
}

t_lst		*dir_analyze(t_ls_opts *opts, char *path, t_lst **flst)
{
	DIR				*d;
	struct dirent	*ent;
	t_lst			*dirs;
	char			*tmp;

	dirs = NULL;
	d = get_dir(opts, path);
	while ((ent = readdir(d)))
		if (!is_hidden(ent->d_name) || opts->opts.a)
		{
			tmp = path_cat(path, ent->d_name);
			if (is_directory(tmp))
				lst_append(&dirs, tmp);
			lst_append(flst, analyze_path(opts, path, ent->d_name));
		}
	(void)closedir(d);
	return (dirs);
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
