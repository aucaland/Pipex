/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:02:56 by aurel             #+#    #+#             */
/*   Updated: 2023/01/27 16:30:37 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files_bonus/pipex_bonus.h"

void	exit_pipex(t_pipex *px, char *err, char *location, int cleaned)
{
	if (cleaned == 5)
	{
		ft_putendl_fd(err, 2);
		exit(0);
	}
	else if (ft_strlen(err) > 0 && ft_strncmp(err, PERROR, 6) == 0)
		perror("");
	else if (ft_strlen(err) > 0)
	{
		ft_putstr_fd(err, 2);
		ft_putendl_fd(location, 2);
	}
	free_pipex(px, cleaned);
}

char	**ft_free_tab_pipex(char **tab, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (tab[i])
			free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

void	free_pipex(t_pipex *px, int cleaned)
{
	if (access(".here_doc.txt", F_OK) == 0)
		unlink(".here_doc.txt");
	if (px->infile != -1)
		close(px->infile);
	if (px->outfile != -1)
		close(px->outfile);
	if (!cleaned)
	{
		if (px)
			free(px);
		exit(EXIT_SUCCESS) ;
	}
	if (px->cmd)
		ft_free_tab_pipex(px->cmd, px->nb_cmd);
	if (px->cmd_paths)
		ft_free_tab_pipex(px->cmd_paths, px->nb_cmd);
	if (px->cmd_args)
	{
		while ((px->nb_cmd--) > 0 && px->cmd_args[px->nb_cmd])
			ft_free_tab(px->cmd_args[px->nb_cmd]);
		free(px->cmd_args);
	}
	if (px->env_paths)
		ft_free_tab(px->env_paths);
	free(px);
	exit(EXIT_SUCCESS);
}
