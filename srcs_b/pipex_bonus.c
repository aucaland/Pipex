/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/25 21:20:59 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files_bonus/pipex_bonus.h"
#define PROMPT_HDOC "> "

int	check_here_doc(t_pipex **px, char **argv)
{
	char	*here_doc;

	here_doc = HERE_DOC;
	*px = malloc(sizeof(t_pipex));
	if (!*px)
		exit_pipex(*px, MALLOC, "init_struct_values", 0);
	if (ft_strlen(argv[1]) == 8 && ft_strncmp(argv[1], here_doc, 8) == 0)
	{
		(*px)->here_doc = 1;
		(*px)->limiter = argv[2];
		return (1);
	}
	(*px)->here_doc = 0;
	return (0);
}

void	init_struct_values(t_pipex **px, int argc, char **argv, char **envp)
{
	char	**args;

	if (!*px)
		*px = malloc(sizeof(t_pipex));
	if (!*px)
		exit_pipex(*px, MALLOC, "init_struct_values", 0);
	if (!(*px)->here_doc)
		(*px)->here_doc = 0;
	args = argv + 2 + (*px)->here_doc;
	clean_px(*px);
	(*px)->nb_cmd = argc - 3 - (*px)->here_doc / 1;
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
	get_cmd_paths((*px));
}

void	here_doc(t_pipex **px, int argc, char **argv, char **envp)
{
	char	*buf;

	(*px)->infile = open(".here_doc.txt", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if ((*px)->infile == -1)
		exit_pipex(*px, "HERE_DOC FAILED", "in function here_doc", 0);
	while (1)
	{
		if (write(STDIN_FILENO, PROMPT_HDOC, ft_strlen(PROMPT_HDOC)) == -1)
			exit_pipex(*px, "", "", 0);
		buf = get_next_line(STDIN_FILENO);
		if (!buf || ft_strcmp(buf, (*px)->limiter) == 0)
		{
			if (!buf)
				write(STDIN_FILENO, "\n", 1);
			close((*px)->infile);
			break ;
		}
		write((*px)->infile, buf, ft_strlen(buf));
		write((*px)->infile, "\n", 1);
		free(buf);
	}
	free(buf);
	close((*px)->infile);
	init_struct_values(px, argc, argv, envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*px;
	int		i;

	i = -1;
	px = NULL;
	if (argc < 5)
		exit_pipex(px, ARGC, "main", 0);
	if (check_here_doc(&px, argv) > 0)
		here_doc(&px, argc, argv, envp);
	else
		init_struct_values(&px, argc, argv, envp);
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
	free_pipex(px, 1);
	exit(EXIT_SUCCESS);
}
