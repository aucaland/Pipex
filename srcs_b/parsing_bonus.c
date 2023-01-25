/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:26:56 by aucaland          #+#    #+#             */
/*   Updated: 2023/01/25 21:16:23 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files_bonus/pipex_bonus.h"

void	get_full_path(t_pipex *px)
{
	char	**tmp;
	char	*env_full_path;
	int		i;

	i = -1;
	tmp = px->env;
	if (!path_exist(px, tmp, &env_full_path))
	{
		ft_putstr_fd("Don't touch my env", 2);
		dprintf(2, "%s", px->env_paths[0]);
		return ;
//		exit_pipex(px, "", "", 1);
	}
	tmp = ft_split(env_full_path, ':');
	free(env_full_path);
	if (!tmp)
		exit_pipex(px, MALLOC, "get_full_path", 1);
	px->env_paths = ft_calloc(sizeof(char *), ft_strlen_tab(tmp) + 1);
	if (!px->env_paths)
		exit_pipex(px, MALLOC, "get full path", 1);
	while (tmp[++i])
		px->env_paths[i] = ft_strjoin(tmp[i], "/");
	ft_free_tab(tmp);
}

void	get_cmd_paths(t_pipex *px)
{
	char	**tmp;
	char	*join_cmd;
	int		i;

	tmp = px->env_paths;
	while (*tmp)
	{
		i = -1;
		while (++i < px->nb_cmd)
		{
			if (access(px->cmd[i], X_OK) == 0 && !px->cmd_paths[i])
				px->cmd_paths[i] = ft_strdup(px->cmd[i]);
			else if (!px->cmd_paths[i])
			{
				join_cmd = ft_strjoin(*tmp, px->cmd[i]);
				if (!join_cmd && px->cmd[i])
					exit(1);
				if (access(join_cmd, X_OK) == 0)
					px->cmd_paths[i] = join_cmd;
				else
					free(join_cmd);
			}
		}
		tmp++;
	}
}

void	get_cmds(t_pipex *px, char **args)
{
	char	**split_cmd;
	int		i;

	i = -1;
	while (++i < px->nb_cmd)
	{
		px->cmd_paths[i] = NULL;
		if (args[i][0] == '\0')
		{
			px->cmd[i] = NULL;
			continue ;
		}
		split_cmd = ft_split(args[i], ' ');
		if (!split_cmd[0])
			exit_pipex(px, MALLOC, "get_cmds", 1);
		px->cmd[i] = ft_strdup(split_cmd[0]);
		if (!px->cmd[i])
			exit_pipex(px, MALLOC, "", 1);
		ft_free_tab(split_cmd);
	}
}

void	get_files(t_pipex *px, char **argv, int argc)
{
	if (px->here_doc == 0)
		px->infile = open(argv[1], O_RDONLY);
	else
		px->infile = open(".here_doc.txt", O_RDONLY);
	if (px->infile == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
	if (px->here_doc == 0)
		px->outfile = open(argv[argc - 1], O_TRUNC | O_WRONLY | O_CREAT, 0644);
	else
		px->outfile = open(argv[argc - 1], O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (px->outfile == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": ", 2);
		perror(" ");
	}
}

void	get_cmds_args(t_pipex *px, char **args)
{
	int	i;

	i = -1;
	px->cmd_args = malloc(sizeof(char **) * px->nb_cmd);
	if (!px->cmd_args)
		exit_pipex(px, MALLOC, "get_cmds_args", 1);
	while (++i < px->nb_cmd)
	{
		px->cmd_args[i] = ft_split(args[i], ' ');
		if (!px->cmd_args[i])
			exit_pipex(px, MALLOC, "get_cmds_args", 1);
	}
}
