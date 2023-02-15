/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 21:48:11 by aurel             #+#    #+#             */
/*   Updated: 2023/02/10 22:13:00 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/pipex.h"

int	is_space(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = ft_strlen(str);
	while (*str == ' ')
	{
		i--;
		++str;
	}
	if (i == 0)
		return (1);
	return (0);
}

void	fill_cmd(t_pipex *px, char **args, char **split_cmd, int i)
{
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
