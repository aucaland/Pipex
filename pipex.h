#ifndef PIPEX_H
#define PIPEX_H

#include "LIBFT/libft.h"
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h> //TODO : erase
#include <errno.h>
#include "errors.h"

void	ft_exit_errors(int count, ...);
void	ft_exit_free(int count, ...);

typedef struct s_child
{
	int nbr;
	pid_t pid;

}						t_child;

typedef struct s_pipex
{
	int infile;
	int outfile;
	char **cmd;
	char ***cmd_args;
	char **env;
	char **env_paths;
	char **cmd_paths;
	int nb_cmd;
	int nb_pipes;
	t_child *child;
	int (pipes_fd)[2];

}				t_pipex;


#endif //PIPEX_PIPEX_H
