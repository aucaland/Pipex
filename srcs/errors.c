/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:02:56 by aurel             #+#    #+#             */
/*   Updated: 2023/01/26 18:04:05 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/pipex.h"

void	exit_pipex(t_pipex *px, char *err, char *location, int cleaned)
{
	if (cleaned == 5)
	{
		ft_putendl_fd(err, 2);
		exit(0);
	}
	if (ft_strlen(err) > 0 && ft_strncmp(err, PERROR, 6))
	{
		perror("");
		free_pipex(px, cleaned);
		return ;
	}
	if (ft_strlen(err) > 0)
	{
		ft_putstr_fd(err, 2);
		ft_putendl_fd(location, 2);
	}
	if (px)
		free_pipex(px, cleaned);
	exit(EXIT_FAILURE);
}

void	free_pipex(t_pipex *px, int cleaned)
{
	if (!cleaned)
	{
		free(px);
		return ;
	}
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
