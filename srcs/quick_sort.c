#include "ft_ls.h"

void	swap(void **a, void **b)
{
	void *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int		ft_strcmp_case(const char *s1, const char *s2)
{
	size_t	k;
	unsigned char c1;
	unsigned char c2;

	k = -1;
	while (s1[++k] || s2[k])
	{
		c1 = ft_tolower(s1[k]);
		c2 = ft_tolower(s2[k]);
		if (c1 != c2)
			return (c1 - c2);
	}
	return (0);
}

int		compare(t_ls_opts *opts, t_long_item *i1, t_long_item *i2)
{
	int	ret_val;

	if (opts->opts & OPT_U)
		ret_val = i1->st->st_atime > i2->st->st_atime;
	else
		ret_val = ft_strcmp(i1->name, i2->name) <= 0;
	return (ret_val);
}

void	quicksort_rec(t_ls_opts *opts, t_long_item **array, int low, int high)
{
	int pivot_index = high;
	int border = low;

	if (low < high)
	{
		for (int j = low; j < high; j++)
			if (compare(opts, array[j], array[pivot_index]))
				swap((void **)array + border++, (void **)array + j);
		swap((void **)array + border, (void **)array + high);
		quicksort_rec(opts, array, low, border - 1);
		quicksort_rec(opts, array, border + 1, high);
	}
}

void	quicksort(t_ls_opts *opts, t_long_item **array, int size)
{
	quicksort_rec(opts, array, 0, size - 1);
}
