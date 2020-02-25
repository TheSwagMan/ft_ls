/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 18:51:28 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:55:37 by tpotier          ###   ########.fr       */
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
	ent->str.date = date_to_str(ent->stat.st_mtime);
	ent->str.mode_s = 10;
	ent->str.nlink_s = ft_strlen(ent->str.nlink);
	ent->str.owner_s = ft_strlen(ent->str.owner);
	ent->str.group_s = ft_strlen(ent->str.group);
	ent->str.size_s = ft_strlen(ent->str.size);
	ent->str.date_s = 12;
	ent->str.name_s = ft_strlen(ent->str.name);
}

t_ls_entry	*analyze_path(t_ls_opts *opts, char *path, char *filename)
{
	t_ls_entry	*ent;

	if (!(ent = malloc(sizeof(*ent))))
		ls_exit("Malloc error", EXIT_FAT_ERR);
	ent->fullpath = path_cat(path, filename);
	ent->str.name = ft_strdup(filename);
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
	set_fields(opts, ent);
	return (ent);
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
