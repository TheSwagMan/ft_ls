/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   max_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 19:13:35 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 19:13:37 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		init_max(t_entry_str *max)
{
	max->name_s = 0;
	max->mode_s = 0;
	max->nlink_s = 0;
	max->owner_s = 0;
	max->group_s = 0;
	max->size_s = 0;
	max->date_s = 0;
}

t_entry_str	*get_max_size(t_lst *lst)
{
	t_entry_str	*max;
	t_entry_str	tmp;

	if (!(max = malloc(sizeof(*max))))
		ls_exit("Malloc", EXIT_FAT_ERR);
	init_max(max);
	lst_goto_n(&lst, 0);
	while (lst)
	{
		tmp = ((t_ls_entry *)lst->data)->str;
		max->name_s = tmp.name_s > max->name_s ? tmp.name_s : max->name_s;
		max->mode_s = tmp.mode_s > max->mode_s ? tmp.mode_s : max->mode_s;
		max->nlink_s = tmp.nlink_s > max->nlink_s ? tmp.nlink_s : max->nlink_s;
		max->owner_s = tmp.owner_s > max->owner_s ? tmp.owner_s : max->owner_s;
		max->group_s = tmp.group_s > max->group_s ? tmp.group_s : max->group_s;
		max->size_s = tmp.size_s > max->size_s ? tmp.size_s : max->size_s;
		max->date_s = tmp.date_s > max->date_s ? tmp.date_s : max->date_s;
		lst = lst->next;
	}
	return (max);
}
