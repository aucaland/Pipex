/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:02:56 by aurel             #+#    #+#             */
/*   Updated: 2023/01/23 10:26:24 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	exit_pipex(t_pipex *px, char *err, char *location)
{
	if (ft_strncmp(err, PERROR, 6))
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

void	ft_free_pipex(t_pipex *px)
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
	free(px);
}
