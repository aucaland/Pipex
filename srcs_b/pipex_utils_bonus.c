/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:38:45 by aucaland          #+#    #+#             */
/*   Updated: 2023/01/27 16:44:40 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files_bonus/pipex_bonus.h"

void	clean_px(t_pipex *px)
{
	px->cmd = NULL;
	px->cmd_args = NULL;
	px->env = NULL;
	px->env_paths = NULL;
	px->cmd_paths = NULL;
	px->infile = -1;
	px->outfile = -1;
}

void	check_and_dup_infile(t_pipex *px, int *i)
{
	if (px->infile == -1)
	{
		if (pipe(px->pipes_fd) == -1)
			exit_pipex(px, PIPE, "check_and_dup_infile", 1);
		close(px->pipes_fd[1]);
		*i += 1;
	}
	else
	{
		if (dup2(px->infile, STDIN_FILENO) == -1)
			exit_pipex(px, PERROR, "main", 1);
		close(px->infile);
	}
}

int	path_exist(t_pipex *px, char ***tmp, char **env_full_path)
{
	int i;

	i = 0;
	while (*tmp[i])
	{
		if (ft_strlen(*tmp[i]) > 4 && ft_strncmp("PATH", *tmp[i], 4) == 0)
		{
			*env_full_path = ft_substr(*tmp[i], 5, ft_strlen(*tmp[i]));
			if (!*env_full_path)
				exit_pipex(px, MALLOC, "get_full_path", 1);
			*tmp = ft_split(*env_full_path, ':');
			free(*env_full_path);
			if (!*tmp)
				exit_pipex(px, MALLOC, "get_full_path", 1);
			return (1);
		}
		tmp[i]++;
	}
	return (0);
}

void	get_path_if_access(t_pipex *px, char ***tmp, char **join_cmd, int i)
{
	if (px->cmd[i] && access(px->cmd[i], X_OK) == 0 && !px->cmd_paths[i])
	{
		px->cmd_paths[i] = ft_strdup(px->cmd[i]);
		if (!px->cmd_paths[i])
			exit_pipex(px, MALLOC, "get_path_if_access", 1);
	}
	else if (!px->cmd_paths[i])
	{
		*join_cmd = ft_strjoin(**tmp, px->cmd[i]);
		if (!*join_cmd && px->cmd[i])
			exit_pipex(px, MALLOC, "get_cmd_paths", 1);
		if (*join_cmd && access(*join_cmd, X_OK) == 0)
			px->cmd_paths[i] = *join_cmd;
		else
			free(*join_cmd);
	}
}

void	fix_null_path(t_pipex *px)
{
	int	i;

	i = 0;
	while (i < px->nb_cmd)
	{
		if (px->cmd_paths[i] == NULL)
		{
			if (px->cmd[i] != NULL)
			{
				px->cmd_paths[i] = ft_strdup(px->cmd[i]);
				if (px->cmd_paths[i] == NULL)
					exit_pipex(px, MALLOC, "fix_null_path", 1);
			}
			else
			{
				px->cmd_paths[i] = malloc(sizeof(char) * 1);
				px->cmd_paths[i][0] = '\0';
			}
		}
		i++;
	}
}