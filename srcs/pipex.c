/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/28 15:41:09 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/pipex.h"

void	init_struct_values(t_pipex **px, int argc, char **argv, char **envp)
{
	char	**args;

	args = argv + 2;
	*px = malloc(sizeof(t_pipex));
	if (!*px)
		exit_pipex(*px, MALLOC, "init_struct_values", 0);
	clean_px(*px);
	(*px)->nb_cmd = argc - 3;
	(*px)->cmd = ft_calloc(sizeof(char *), (*px)->nb_cmd + 1);
	if (!(*px)->cmd)
		exit_pipex(*px, MALLOC, "get_cmds", 1);
	(*px)->cmd_paths = ft_calloc(sizeof(char *), (*px)->nb_cmd + 1);
	if (!(*px)->cmd_paths)
		exit_pipex(*px, MALLOC, "get_cmds", 1);
	(*px)->env = envp;
	get_cmds((*px), args);
	get_cmds_args((*px), args);
	get_files((*px), argv, argc);
	get_full_path(*px);
	if ((*px)->env_paths)
		get_cmd_paths((*px));
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*px;
	int		i;

	i = -1;
	px = NULL;
	if (argc != 5)
		exit_pipex(px, ARGC, "", 5);
	init_struct_values(&px, argc, argv, envp);
	check_and_dup_infile(px, &i);
	while (++i < px->nb_cmd)
	{
		make_child(px, i);
		if (i < px->nb_cmd - 1)
			close(px->pipes_fd[1]);
	}
	close(STDIN_FILENO);
	while (waitpid(-1, NULL, 0) > 0)
		;
	free_pipex(px, 1);
	exit(EXIT_SUCCESS);
}
