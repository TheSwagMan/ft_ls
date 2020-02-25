/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:08:52 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:08:54 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

char		is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
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
