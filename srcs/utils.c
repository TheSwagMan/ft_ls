/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 20:05:16 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/09 15:40:56 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

size_t		term_size(void)
{
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if (!w.ws_col)
		return (80);
	return (w.ws_col);
}

void		ls_exit(char *msg, char code)
{
	if (errno)
		perror(msg);
	else
		ft_putendl_fd(msg, STDERR_FILENO);
	exit(code);
}

#ifdef __APPLE__

time_t		get_birthtime(struct stat st)
{
	return (st.st_birthtime);
}

#else

time_t		get_birthtime(struct stat st)
{
	return (st.st_ctime);
}

#endif
