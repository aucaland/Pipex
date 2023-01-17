/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:48:25 by aurel             #+#    #+#             */
/*   Updated: 2023/01/17 15:48:25 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void close_fds(int count, ...)
{
	va_list ap;
	va_start(ap, count);

	for (int i = 0; i < count; i++) {
		int fd = va_arg(ap, int);
		close(fd);
	}
	va_end(ap);
}