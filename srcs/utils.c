/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 20:05:16 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/03 19:14:28 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

size_t		term_size(void)
{
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
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
