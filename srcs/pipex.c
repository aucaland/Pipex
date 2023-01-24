/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/24 21:13:30 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
#define PROMPT_HERE_DOC "> "

int check_here_doc(t_pipex **px, char **argv)
{
	char	*here_doc;

	here_doc = HERE_DOC;
	*px = malloc(sizeof(t_pipex));
	if (!*px)
		exit_pipex(*px, MALLOC, "init_struct_values");
	if (ft_strlen(argv[1]) == 8 && ft_strncmp(argv[1], here_doc, 8) == 0)
	{
		(*px)->here_doc = 1;
		(*px)->limiter = argv[2];
		return (1);
	}
	(*px)->here_doc = 0;
	return (0);
}

t_pipex	*init_struct_values(t_pipex **px, int argc, char **argv, char **envp)
{
	char	**args;
	char	*env_full_path;

	if (!*px)
		*px = malloc(sizeof(t_pipex));
	if (!*px)
		exit_pipex(*px, MALLOC, "init_struct_values");
	if (!(*px)->here_doc)
		(*px)->here_doc = 0;
	args = argv + 2 + (*px)->here_doc;
	env_full_path = NULL;
	clean_px(*px);
	(*px)->nb_cmd = argc - 3 - (*px)->here_doc / 1;
	(*px)->env = envp;
	get_cmds((*px), args);
	get_cmds_args((*px), args);
	get_files((*px), argv, argc);
	get_full_path(*px, env_full_path);
	get_cmd_paths((*px));
	return ((*px));
}

t_pipex *here_doc(t_pipex **px, int argc, char **argv, char **envp)
{
	char	*buf;

	(*px)->infile = open(".here_doc.txt", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if ((*px)->infile == -1)
		exit_pipex(*px, "HERE_DOC FAILED", "in function here_doc");
	while (1)
	{
		if (write(STDIN_FILENO, PROMPT_HERE_DOC, ft_strlen(PROMPT_HERE_DOC)) == -1)
			exit(1);
		buf = get_next_line(STDIN_FILENO);
		if (!buf)
			exit_pipex(*px, MALLOC, "here_doc");
		if (ft_strcmp(buf, (*px)->limiter) == 0)
			break ;
		write((*px)->infile, buf, ft_strlen(buf));
		write((*px)->infile, "\n", 1);
		free(buf);
	}
	free(buf);
	close((*px)->infile);
	init_struct_values(px, argc, argv, envp);
	return (*px);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*px;
	int		i;

	i = -1;
	px = NULL;
	if (argc < 5 || !envp)
		exit(1);
	if (argc == 6 && check_here_doc(&px, argv) > 0)
		px = here_doc(&px, argc, argv, envp);
	else
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
