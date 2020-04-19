/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 15:19:25 by user42            #+#    #+#             */
/*   Updated: 2020/04/19 15:19:32 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		disp_attr(char *name, int size)
{
	char	*snbr;

	ft_putchar('\t');
	ft_putstr(name);
	ft_putchar('\t');
	snbr = ft_itoa(size);
	if (5 - ft_strlen(snbr) > 0)
		ft_putnchar(' ', 5 - ft_strlen(snbr));
	ft_putstr(snbr);
	free(snbr);
	ft_putchar('\n');
}

#ifdef __APPLE__

void		display_xattr(t_ls_opts *opts, t_ls_entry *ent)
{
	char	*s;
	ssize_t	n;
	ssize_t	i;

	i = 0;
	if (opts->opts.aaa && (n = listxattr(ent->fullpath, NULL, 0,
					XATTR_NOFOLLOW)) > 0)
	{
		if (!(s = malloc(n * sizeof(*s))))
			ls_exit("malloc failed in display_xattr", EXIT_FAT_ERR);
		listxattr(ent->fullpath, s, n, XATTR_NOFOLLOW);
		while (i < n)
		{
			disp_attr(&(s[i]), getxattr(ent->fullpath, &(s[i]), NULL, 0, 0,
						XATTR_NOFOLLOW));
			while (s[i++])
				continue;
		}
		free(s);
	}
}

#else

void		display_xattr(t_ls_opts *opts, t_ls_entry *ent)
{
	char	*s;
	ssize_t	n;
	ssize_t	i;

	i = 0;
	if (opts->opts.aaa && (n = llistxattr(ent->fullpath, NULL, 0)) > 0)
	{
		if (!(s = malloc(n * sizeof(*s))))
			ls_exit("malloc failed in display_xattr", EXIT_FAT_ERR);
		llistxattr(ent->fullpath, s, n);
		while (i < n)
		{
			disp_attr(&(s[i]), lgetxattr(ent->fullpath, &(s[i]), NULL, 0));
			while (s[i++])
				continue;
		}
		free(s);
	}
}

#endif
