/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 05:28:13 by tpotier           #+#    #+#             */
/*   Updated: 2020/01/24 19:43:17 by tpotier          ###   ########.fr       */
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

typedef struct	s_itm_parsd
{
	char		*rules;
	size_t		rules_s;
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
}				t_itm_parsd;

typedef struct	s_long_item
{
	struct stat	*st;
	char		*path;
	char		*name;
	char		*link_name;
	t_itm_parsd	*par;
}				t_long_item;

typedef struct	s_ls_entry
{
	struct stat	stat;
	struct stat	rstat;
	char		*name;
}				t_ls_entry;

#endif
