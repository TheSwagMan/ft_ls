/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:14:25 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/09 15:41:50 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			sort_by_name(void *e1, void *e2)
{
	return (ft_strcmp(((t_ls_entry *)e1)->str.name,
				((t_ls_entry *)e2)->str.name) > 0);
}

int			sort_by_mdate(void *e1, void *e2)
{
	if (((t_ls_entry *)e1)->stat.st_mtime
			== ((t_ls_entry *)e2)->stat.st_mtime)
		return (sort_by_name(e1, e2));
	return (((t_ls_entry *)e1)->stat.st_mtime
			< ((t_ls_entry *)e2)->stat.st_mtime);
}

int			sort_by_adate(void *e1, void *e2)
{
	return (((t_ls_entry *)e1)->stat.st_atime
			< ((t_ls_entry *)e2)->stat.st_atime);
}

int			sort_by_cdate(void *e1, void *e2)
{
	return (((t_ls_entry *)e1)->stat.st_birthtime
			< ((t_ls_entry *)e2)->stat.st_birthtime);
}

void		sort_entry_list(t_ls_opts *opts, t_lst **lst,
		int (*f)(void *e1, void *e2))
{
	t_lst	*new;

	if (!lst || !*lst)
		return ;
	new = NULL;
	lst_goto_n(lst, 0);
	while (*lst)
		if (opts->opts.r)
			lst_insert_sorted_rev(&new, lst_pop(lst), f);
		else
			lst_insert_sorted(&new, lst_pop(lst), f);
	*lst = new;
}
