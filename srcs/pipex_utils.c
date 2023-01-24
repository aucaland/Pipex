/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:38:45 by aucaland          #+#    #+#             */
/*   Updated: 2023/01/24 20:43:55 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	clean_px(t_pipex *px)
{
	px->cmd = NULL;
	px->cmd_args = NULL;
	px->env = NULL;
	px->env_paths = NULL;
	px->cmd_paths = NULL;
}

void	check_and_dup_infile(t_pipex *px, int *i)
{
	if (px->infile == -1)
	{
		if (pipe(px->pipes_fd) == -1)
			exit_pipex(px, PIPE, "check_and_dup_infile");
		close(px->pipes_fd[1]);
		*i += 1;
	}
	else
	{
		if (dup2(px->infile, STDIN_FILENO) == -1)
			exit_pipex(px, PERROR, "main");
		close(px->infile);
	}
}
