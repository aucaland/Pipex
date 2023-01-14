/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/13 11:49:49 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	int pid = fork();

	if (pid == 0)
	{
		kill(pid, SIGSTOP);
		sleep(1);
		kill(pid, SIGCONT);
		printf("Some message\n");
		kill(pid, SIGKILL);
	}
	else
	{
		//sleep(1);
		kill(pid, SIGSTOP);
		sleep(1);
		kill(pid, SIGCONT);

		wait(NULL);

	}
	return (0);
	//printf("Salut from id %d\n", id);
//	if (id == 0)
//	{
//		printf("Salut from child process ID = %d\n", id);
//	}
//	else
//		printf("Salut from parent process ID = %d\n", id);
	return 0;
}