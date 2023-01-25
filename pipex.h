#ifndef PIPEX_H
#define PIPEX_H

#include "LIBFT/libft.h"
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h> //TODO : erase
#include <errno.h>
#include "errors.h"

#define HERE_DOC "here_doc"
typedef struct s_pipex
{
	int		infile;
	int		outfile;
	char	**cmd;
	char	***cmd_args;
	char	**env;
	char	**env_paths;
	char	**cmd_paths;
	int		nb_cmd;
	int		pipes_fd[2];
	int		pid;
	int		here_doc;
	char	*limiter;
}				t_pipex;

void		exit_pipex(t_pipex *px, char *err, char *location, int cleaned);
void		free_pipex(t_pipex *px, int cleaned);
void		clean_px(t_pipex *px);
void		do_in_child(t_pipex *px, int nbr);
void		make_child(t_pipex *px, int nbr);
void		check_and_dup_infile(t_pipex *px, int *i);

void		get_full_path(t_pipex *px, char *env_full_path);
void		get_cmd_paths(t_pipex *px);
void		get_cmds(t_pipex *px, char **args);
void		get_files(t_pipex *px, char **argv, int argc);
void		get_cmds_args(t_pipex *px, char **args);
#endif
