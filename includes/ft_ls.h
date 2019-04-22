#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <time.h>
# include <stdio.h>
# include <stdlib.h>
# include <pwd.h>
# include <grp.h>

# include "libft.h"

# define		OPT_L	(1 << 0)
# define		OPT_RR	(1 << 1)
# define		OPT_A	(1 << 2)
# define		OPT_R	(1 << 3)
# define		OPT_T	(1 << 4)
# define		OPT__N	(1 << 5)
# define		OPT_N	(1 << 6)
# define		OPT_U	(1 << 7)
# define		OPT_1	(1 << 8)
# define		OPT_GG	(1 << 9)

# define		EXIT_ERR		1
# define		EXIT_FAT_ERR	2

# define		FNAME_SIZE		255

typedef struct	s_ls_opts
{
	char		*name;
	long		opts;
	char		**paths;
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

void	quicksort(t_ls_opts *opts, t_long_item **array, int size);

#endif
