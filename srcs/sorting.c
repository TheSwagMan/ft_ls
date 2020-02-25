#include "ft_ls.h"

int			sort_by_name(void *e1, void *e2)
{
	return (ft_strcmp(((t_ls_entry *)e1)->str.name, ((t_ls_entry *)e2)->str.name) > 0);
}

int			sort_by_date(void *e1, void *e2)
{
	return (((t_ls_entry *)e1)->stat.st_mtime < ((t_ls_entry *)e2)->stat.st_mtime);
}

void		sort_entry_list(t_lst **lst, int (*f)(void *e1, void *e2))
{
	t_lst	*new;

	if (!lst || !*lst)
		return ;
	new = NULL;
	lst_goto_n(lst, 0);
	while (*lst)
		lst_insert_sorted(&new, lst_pop(lst), f);
	*lst = new;
}

