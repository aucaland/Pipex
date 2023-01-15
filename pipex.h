#ifndef PIPEX_H
#define PIPEX_H

#include "LIBFT/libft.h"
#include <sys/wait.h>

typedef struct s_pipex
{
	int infile;
	int outfile;
	char **cmd;
	char **cmd_args;
	char **env;
	char **env_paths;
	char **cmd_paths;
	int nb_cmd;
	int nb_pipes;

}	t_pipex;
#endif //PIPEX_PIPEX_H
