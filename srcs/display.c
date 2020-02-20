#include "ft_ls.h"

void		display_entry_list(t_lst *lst, t_entry_str *max)
{
	t_ls_entry		*ent;
	char			*name;
	int				size;

	lst_goto_n(&lst, 0);
	while (lst)
	{
		ent = lst->data;
		ft_putstr(ent->str.mode);
		ft_putnchar(' ', 2 + (max->nlink_s - ent->str.nlink_s));
		ft_putstr(ent->str.nlink);
		ft_putchar(' ');
		ft_putstr(ent->str.owner);
		ft_putnchar(' ', 2 + (max->owner_s - ent->str.owner_s));
		ft_putstr(ent->str.group);
		ft_putnchar(' ', 1 + (max->group_s - ent->str.group_s));
		ft_putnchar(' ', 1 + (max->size_s - ent->str.size_s));
		ft_putstr(ent->str.size);
		ft_putchar(' ');
		ft_putstr(ent->str.date);
		ft_putchar(' ');
		ft_putstr(ent->name);
		if (S_ISLNK(ent->stat.st_mode))
		{
			size = ent->stat.st_size ? ent->stat.st_size : PATH_MAX;
			if (!(name = malloc(size + 1)))
				ls_exit("Malloc", EXIT_FAT_ERR);
			if (readlink(ent->fullpath, name, size) != ent->stat.st_size && ent->stat.st_size)
				ls_exit("readlink", EXIT_FAT_ERR);
			name[size] = '\0';
			ft_putstr(" -> ");
			ft_putstr(name);
			free(name);
		}
		ft_putchar('\n');
		lst = lst->next;
	}
}

void		ls_disp_job(t_lst *lst)
{
	t_entry_str	*max;

	sort_entry_list(&lst, sort_by_name);
	max = get_max_size(lst);
	display_entry_list(lst, max);
	free(max);
}
