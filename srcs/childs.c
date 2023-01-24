/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 12:01:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/23 10:25:19 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	do_in_child(t_pipex *px, int nbr)
{
	close(px->pipes_fd[0]);
	if (nbr + 1 < px->nb_cmd)
	{
		if (dup2(px->pipes_fd[1], STDOUT_FILENO) == -1)
			exit_pipex(px, PERROR, "do_in_child");
		close(px->pipes_fd[1]);
	}
	else
	{
		if (dup2(px->outfile, STDOUT_FILENO) == -1)
			exit(0);
		close(px->outfile);
		close(px->pipes_fd[1]);
	}
	execve(px->cmd_paths[nbr], px->cmd_args[nbr], px->env);
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(px->cmd_args[nbr][0], 2);
	ft_putendl_fd(": command not found", 2);
	exit (1);
}

void	make_child(t_pipex *px, int nbr)
{
	if (nbr > 0)
	{
		if (px->pipes_fd[0] != -1 && dup2(px->pipes_fd[0], STDIN_FILENO) == -1)
		{
			close_fds(2, px->pipes_fd[0], px->pipes_fd[1]);
			exit_pipex(px, PERROR, "make_child");
		}
		close(px->pipes_fd[0]);
	}
	if (nbr < px->nb_cmd - 1 && pipe(px->pipes_fd) == -1)
		exit_pipex(px, DUP2, "make_child");
	px->pid = fork();
	if (px->pid == -1)
		exit_pipex(px, PERROR, "make_child");
	if (px->pid)
		return ;
	if (!px->pid)
		do_in_child(px, nbr);
}
