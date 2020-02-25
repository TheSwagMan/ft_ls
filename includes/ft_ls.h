/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 05:28:13 by tpotier           #+#    #+#             */
/*   Updated: 2020/02/25 20:40:18 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <dirent.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <pwd.h>
# include <grp.h>
# include <errno.h>
# include "libft.h"
# include "liblst.h"

# define EXIT_ERR		1
# define EXIT_FAT_ERR	2

# define SEC_IN_6_MON	15552000

typedef struct	s_opts
{
	uint8_t		l : 1;
	uint8_t		rr : 1;
	uint8_t		a : 1;
	uint8_t		r : 1;
	uint8_t		t : 1;
	uint8_t		n_ : 1;
	uint8_t		n : 1;
	uint8_t		u : 1;
	uint8_t		uu : 1;
	uint8_t		o1 : 1;
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
	char		*name;
	size_t		name_s;
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

typedef struct	s_ls_entry
{
	struct stat	stat;
	struct stat	rstat;
	char		*fullpath;
	t_entry_str	str;
}				t_ls_entry;

void			add_opt(t_ls_opts *opts, char *sopts);
int				sort_by_name(void *e1, void *e2);
int				sort_by_mdate(void *e1, void *e2);
int				sort_by_adate(void *e1, void *e2);
int				sort_by_cdate(void *e1, void *e2);
void			sort_entry_list(t_ls_opts *opts, t_lst **lst,
		int (*f)(void *e1, void *e2));
void			parse_opts(t_ls_opts *opts, int ac, char **av);
void			init_opts(t_opts *opts);
t_ls_opts		*init_ls_opts(int ac, char **av);

char			*mode_to_str(mode_t mode);
char			*owner_to_str(uid_t uid);
char			*group_to_str(gid_t gid);
char			*date_to_str(time_t tm);
char			*format_majmin(dev_t rdev);
void			ls_exit(char *msg, char code);
int				is_hidden(char *path);
int				paths_count(int ac, char **av);
char			*path_cat(char *dir, char *file);
t_ls_entry		*analyze_path(t_ls_opts *opts, char *path, char *filename);
void			free_ls_entry(void *tmp);
DIR				*get_dir(t_ls_opts *opts, char *path);
int				dir_analyze(t_ls_opts *opts, char *path, t_lst **flst);
char			is_directory(char *path);
t_entry_str		*get_max_size(t_lst *lst);
void			ls_disp_job(t_ls_opts *opts, t_lst *lst);
int				total_dir(t_lst *lst);
void			ls(char *path, t_ls_opts *opts);
t_lst			*analyze_path_lst(t_ls_opts *opts, t_lst *lst);
size_t			term_size(void);

#endif
