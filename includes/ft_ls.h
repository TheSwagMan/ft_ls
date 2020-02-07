/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 05:28:13 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/07 14:39:17 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <time.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pwd.h>
# include <grp.h>
# include <errno.h>


# include "libft.h"
# include "liblst.h"

# define EXIT_ERR		1
# define EXIT_FAT_ERR	2

# define FNAME_SIZE		255

# define SEC_IN_6_MON	(60 * 60 * 24 * 30 * 6)

typedef struct	s_opts
{
	uint8_t		l : 1;
	uint8_t		rr : 1;
	uint8_t		a : 1;
	uint8_t		r : 1;
	uint8_t		t : 1;
	uint8_t		_n : 1;
	uint8_t		n : 1;
	uint8_t		u : 1;
	uint8_t		uu : 1;
	uint8_t		_1 : 1;
	uint8_t		gg : 1;
}				t_opts;

typedef struct	s_ls_opts
{
	char		*name;
	t_opts		opts;
	t_lst		*dpaths;
	t_lst		*fpaths;
}				t_ls_opts;

typedef struct	s_entry_str
{
	char		*mode;
	size_t		mode_s;
	char		*nlink;
	size_t		nlink_s;
	char		*owner;
	size_t		owner_s;
	char		*group;
	size_t		group_s;
	char		*size;
	size_t		size_s;
	char		*date;
	size_t		date_s;
}				t_entry_str;

typedef struct	s_long_item
{
	struct stat	*st;
	char		*path;
	char		*name;
	char		*link_name;
	t_entry_str	*par;
}				t_long_item;

typedef struct	s_ls_entry
{
	struct stat	stat;
	struct stat	rstat;
	char		*name;
	t_entry_str	str;
}				t_ls_entry;

#endif
