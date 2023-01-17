/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:02:56 by aurel             #+#    #+#             */
/*   Updated: 2023/01/17 17:11:34 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void ft_exit_errors(int count, ...)
{
	va_list ap;
	va_start(ap, count);

	for (int i = 0; i < count; i++)
		ft_printf("%s", va_arg(ap, int));
	va_end(ap);
	exit(EXIT_FAILURE);
}

void ft_exit_free(int count, ...)
{
	va_list ap;
	va_start(ap, count);

	for (int i = 0; i < count; i++) {
		int fd = va_arg(ap, int);
		close(fd);
	}
	va_end(ap);
}