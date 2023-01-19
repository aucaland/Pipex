/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:02:56 by aurel             #+#    #+#             */
/*   Updated: 2023/01/19 22:53:48 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clean_px(t_pipex *px)
{
	px->cmd = NULL;
	px->cmd_args = NULL;
	px->env = NULL;
	px->env_paths = NULL;
	px->cmd_paths = NULL;
}

void	ft_exit_pipex(t_pipex *px, char *err, char *location)
{
	if (err == PERROR)
	{
		perror("");
		ft_free_pipex(px);
		return ;
	}
	ft_putstr_fd(err, 2);
	ft_putendl_fd(location, 2);
	if (px)
		ft_free_pipex(px);
	exit(EXIT_FAILURE);
}

void ft_free_pipex(t_pipex *px)
{
	if (px->cmd)
		ft_free_tab(px->cmd);
	if (px->cmd_args)
	{
		while ((px->nb_cmd)-- > 0)
			ft_free_tab(px->cmd_args[px->nb_cmd]);
		free(px->cmd_args);
	}
	if (px->env_paths)
		ft_free_tab(px->env_paths);
	if (px->cmd_paths)
		ft_free_tab(px->cmd_paths);
	if (px->child)
		free(px->child);
	free(px);
}

