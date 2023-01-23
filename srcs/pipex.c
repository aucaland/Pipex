/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/23 10:46:30 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

t_pipex	*init_struct_values(t_pipex **px, int argc, char **argv, char **envp)
{
	char	**args;
	char	*env_full_path;

	args = argv + 2;
	env_full_path = NULL;
	*px = malloc(sizeof(t_pipex));
	if (!*px)
		ft_exit_pipex(*px, MALLOC, "init_struct_values");
	clean_px(*px);
	(*px)->nb_cmd = argc - 3;
	(*px)->env = envp;
	get_cmds((*px), args);
	get_cmds_args((*px), args);
	get_files((*px), argv, argc);
	get_full_path(*px, env_full_path);
	get_cmd_paths((*px));
	return ((*px));
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*px;
	int		i;

	i = -1;
	px = NULL;
	if (argc < 5 || !envp)
		ft_exit_pipex(NULL, ARGC, "");
	px = init_struct_values(&px, argc, argv, envp);
	check_and_dup_infile(px, &i);
	while (++i < px->nb_cmd)
	{
		make_child(px, i);
		if (i < px->nb_cmd - 1)
			close(px->pipes_fd[1]);
		else
			close(px->outfile);
	}
	close(STDIN_FILENO);
	while (waitpid(-1, NULL, 0) > 0)
		;
	ft_free_pipex(px);
	exit(EXIT_SUCCESS);
}
