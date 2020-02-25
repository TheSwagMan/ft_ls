/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:14:25 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:39:15 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			sort_by_name(void *e1, void *e2)
{
	return (ft_strcmp(((t_ls_entry *)e1)->str.name,
				((t_ls_entry *)e2)->str.name) > 0);
}

int			sort_by_date(void *e1, void *e2)
{
	return (((t_ls_entry *)e1)->stat.st_mtime
			< ((t_ls_entry *)e2)->stat.st_mtime);
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
