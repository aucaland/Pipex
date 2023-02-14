/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 12:01:03 by aurel             #+#    #+#             */
/*   Updated: 2023/02/10 22:06:48 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/pipex.h"

void	do_in_child(t_pipex *px, int nbr)
{
	close(px->pipes_fd[0]);
	if (nbr + 1 < px->nb_cmd)
	{
		if (dup2(px->pipes_fd[1], STDOUT_FILENO) == -1)
			exit_pipex(px, PERROR, "do_in_child", 1);
		close_fds(px);
	}
	else
	{
		if (dup2(px->outfile, STDOUT_FILENO) == -1)
			exit(0);
		close_fds(px);
	}
	if (px->cmd[nbr][0] != '\0')
		execve(px->cmd_paths[nbr], px->cmd_args[nbr], px->env);
	else
		exit(0);
	print_cmd_not_found(px, px->cmd[nbr]);
	close_fds(px);
	exit (0);
}

void	make_child(t_pipex *px, int nbr)
{
	if (nbr > 0)
	{
		if (px->pipes_fd[0] != -1 && dup2(px->pipes_fd[0], STDIN_FILENO) == -1)
			exit_pipex(px, DUP2, "make_child", 1);
		close(px->pipes_fd[0]);
	}
	if (nbr < px->nb_cmd - 1 && pipe(px->pipes_fd) == -1)
		exit_pipex(px, PIPE, "make_child", 1);
	px->pid = fork();
	if (px->pid == -1)
		exit_pipex(px, PERROR, "make_child", 1);
	if (px->pid)
		return ;
	if (!px->pid)
		do_in_child(px, nbr);
}
