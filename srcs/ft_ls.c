#include "ft_ls.h"

void		ls_exit(char *msg, char code)
{
	if (errno)
		perror(msg);
	else
		ft_putstr_fd(msg, STDERR_FILENO);
	exit(code);
}

int			is_hidden(char *path)
{
	int	n;

	n = ft_strlen(path);
	while (n && path[n] != '/')
		n--;
	n += n ? 1 : 0;
	return (path[n] == '.' ? 1 : 0);
}

int			paths_count(int ac, char **av)
{
	int	n;

	n = 0;
	while (ac > 1)
		n += (*(av[--ac]) == '-') ? 0 : 1;
	return (n);
}

char		*path_cat(char *dir, char *file)
{
	char	*res;

	if (!(res = malloc(ft_strlen(dir) + ft_strlen(file) + 2)))
		ls_exit("Malloc", EXIT_FAT_ERR);
	*res = '\0';
	ft_strcat(res, dir);
	ft_strcat(res, "/");
	ft_strcat(res, file);
	return (res);
}

void		free_ls_entry(void *tmp)
{
	t_ls_entry	*ent;

	ent = (t_ls_entry *)tmp;
	//ft_printf("freeing %s\n", ent->name);
	free(ent->str.mode);
	free(ent->str.nlink);
	free(ent->str.owner);
	free(ent->str.group);
	free(ent->str.size);
	free(ent->str.date);
	free(ent->fullpath);
	free(ent->name);
	free(ent);
}

void		ls(char *path, t_ls_opts *opts)
{
	t_lst		*lst;

	lst = NULL;
	if (opts->opts._n)
	{
		ft_putstr(path);
		ft_putendl(":");
	}
	dir_analyze(opts, path, &lst);
	ft_putstr("total ");
	ft_putnbr(total_dir(lst));
	ft_putchar('\n');
	ls_disp_job(lst);
	lst_delete(&lst, free_ls_entry);
	/*
	if (opts->opts.l)
		full_long_display(opts, lst, size);
	else if (opts->opts._1)
		full_short_disp(opts, lst, size);
	else
	*/
}

int			main(int ac, char **av)
{
	t_ls_opts	*opts;
	t_lst		*tst;

	opts = init_ls_opts(ac, av);
	tst = analyze_path_lst(opts->fpaths);
	ls_disp_job(tst);
	if (tst && opts->dpaths)
		ft_putchar('\n');
	lst_delete(&tst, free_ls_entry);
	while (opts->dpaths)
	{
		ls(opts->dpaths->data, opts);
		if ((opts->dpaths = opts->dpaths->next))
			ft_putchar('\n');
	}
	return (0);
}
