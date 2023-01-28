/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:26:56 by aucaland          #+#    #+#             */
/*   Updated: 2023/01/27 16:07:14 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/pipex.h"

void	get_full_path(t_pipex *px)
{
	char	**tmp;
	char	*env_full_path;
	int		i;

	i = -1;
	tmp = px->env;
	if (!path_exist(px, &tmp, &env_full_path))
		return ;
	px->env_paths = ft_calloc(sizeof(char *), ft_strlen_tab(tmp) + 1);
	if (!px->env_paths)
	{
		ft_free_tab(tmp);
		exit_pipex(px, MALLOC, "get full path", 1);
	}
	while (tmp[++i])
	{
		px->env_paths[i] = ft_strjoin(tmp[i], "/");
		if (!px->env_paths[i])
		{
			ft_free_tab(tmp);
			exit_pipex(px, MALLOC, "get_full_path", 1);
		}
	}
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
			get_path_if_access(px, &tmp, &join_cmd, i);
		tmp++;
	}
	fix_null_path(px);
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
		if (!split_cmd)
			exit_pipex(px, MALLOC, "get_cmds", 1);
		px->cmd[i] = ft_strdup(split_cmd[0]);
		if (!px->cmd[i])
		{
			ft_free_tab(split_cmd);
			exit_pipex(px, MALLOC, "", 1);
		}
		ft_free_tab(split_cmd);
	}
}

void	get_files(t_pipex *px, char **argv, int argc)
{
	px->infile = open(argv[1], O_RDONLY);
	if (px->infile == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
	px->outfile = open(argv[argc - 1], O_TRUNC | O_WRONLY | O_CREAT, 0644);
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
