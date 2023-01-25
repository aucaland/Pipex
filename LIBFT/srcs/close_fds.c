/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:48:25 by aurel             #+#    #+#             */
/*   Updated: 2023/01/25 14:58:41 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	close_fds(int count, ...)
{
	va_list	ap;
	int		fd;
	int		i;

	i = 0;
	va_start(ap, count);
	while (i < count)
	{
		fd = va_arg(ap, int);
		i++;
	}
	close(fd);
	va_end(ap);
}
