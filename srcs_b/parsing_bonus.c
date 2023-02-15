/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:26:56 by aucaland          #+#    #+#             */
/*   Updated: 2023/02/10 22:14:44 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files_bonus/pipex.h"

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
	split_cmd = NULL;
	while (++i < px->nb_cmd)
	{
		px->cmd_paths[i] = NULL;
		if (is_space(args[i]))
		{
			px->cmd[i] = ft_strdup(args[i]);
			if (!px->cmd[i])
				exit_pipex(px, MALLOC, "get_cmds", 1);
			continue ;
		}
		if (args[i][0] == '\0')
		{
			px->cmd[i] = ft_calloc(sizeof(char), 1);
			px->cmd[i][0] = '\0';
			continue ;
		}
		fill_cmd(px, args, split_cmd, i);
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
		ft_putstr_fd(" : ", 2);
		perror("");
	}
	px->outfile = open(argv[argc - 1], O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (px->outfile == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(argv[argc - 1], 2);
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
