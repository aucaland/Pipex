/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/02 16:12:08 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int id = fork();
	if (id != 0)
		fork();
	printf("Hello world\n");
	//printf("Salut from id %d\n", id);
//	if (id == 0)
//	{
//		printf("Salut from child process ID = %d\n", id);
//	}
//	else
//		printf("Salut from parent process ID = %d\n", id);
	return 0;
}