/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 16:06:02 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/02 16:06:05 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char		*format_majmin(dev_t rdev)
{
	char	*min;
	char	*maj;
	char	*res;
	uint8_t	i;

	if (!(res = ft_memalloc(8)))
		ls_exit("malloc", EXIT_FAT_ERR);
	i = 0;
	maj = ft_itoa(rdev >> 24 & 0xff);
	min = ft_itoa(rdev & 0xff);
	while (i < 3 - ft_strlen(maj))
		res[i++] = ' ';
	ft_strcat(res, maj);
	ft_strcat(res, ", ");
	i = 0;
	while (i < 3 - ft_strlen(min))
		res[5 + i++] = ' ';
	ft_strcat(res, min);
	free(maj);
	free(min);
	return (res);
}
