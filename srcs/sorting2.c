/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 16:59:38 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/03 17:09:17 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		sort_entries(t_ls_opts *opts, t_lst **lst)
{
	if (opts->opts.t)
	{
		if (opts->opts.uu)
			sort_entry_list(opts, lst, sort_by_cdate);
		else if (opts->opts.u)
			sort_entry_list(opts, lst, sort_by_adate);
		else
			sort_entry_list(opts, lst, sort_by_mdate);
	}
	else
		sort_entry_list(opts, lst, sort_by_name);
}
