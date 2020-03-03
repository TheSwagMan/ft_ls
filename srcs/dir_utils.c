/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:08:52 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/03 17:14:03 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

DIR			*get_dir(t_ls_opts *opts, char *path)
{
	DIR	*d;

	if (!(d = opendir(path)))
	{
		ft_putstr_fd(opts->name, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(filename(path), 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (NULL);
	}
	return (d);
}

char		is_directory(char *path)
{
	struct stat	st;

	if (lstat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
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

t_lst		*get_dirs(t_lst *lst)
{
	t_lst			*dirs;
	t_ls_entry		*ent;

	dirs = NULL;
	lst_goto_n(&lst, 0);
	while (lst)
	{
		ent = lst->data;
		if (is_directory(ent->fullpath))
			lst_append(&dirs, ft_strdup(ent->fullpath));
		lst = lst->next;
	}
	return (dirs);
}
