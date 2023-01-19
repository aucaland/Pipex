/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aucaland <aucaland@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/19 22:24:15 by aucaland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	get_full_path(t_pipex *px, char *env_full_path)
{
	char	**tmp;
	int 	i;

	i = -1;
	tmp = px->env;
	while (tmp && !env_full_path)
	{
		if (ft_strncmp("PATH", *tmp, 4) == 0)
		{
			env_full_path = ft_substr(*tmp, 5, ft_strlen(*tmp));
			if (!env_full_path)
				ft_exit_pipex(px, MALLOC, "get_full_path");
		}
		tmp++;
	}
	tmp = ft_split(env_full_path, ':');
	free(env_full_path);
	if (!tmp)
		ft_exit_pipex(px, MALLOC, "get_full_path");
	px->env_paths = ft_calloc(sizeof(char *) , ft_strlen_tab(tmp) + 1);
	if (!px->env_paths)
		ft_exit_pipex(px, MALLOC, "get full path");
	while (tmp[++i])
		px->env_paths[i] = ft_strjoin(tmp[i], "/");
	ft_free_tab(tmp);
}

void get_cmd_paths(t_pipex *px)
{
	char **tmp;
	char *join_cmd;
	int	i;

	tmp = px->env_paths;
	while (*tmp)
	{
		i = -1;
		while (++i < px->nb_cmd)
		{
			if (access(px->cmd[i], X_OK) == 0 && !px->cmd_paths[i])
				px->cmd_paths[i] = ft_strdup(px->cmd[i]);
			else if (!px->cmd_paths[i])
			{
				join_cmd = ft_strjoin(*tmp, px->cmd[i]);
				if (!join_cmd && px->cmd[i])
					exit(1);
				if (access(join_cmd, X_OK) == 0)
					px->cmd_paths[i] = join_cmd;
				else
					free(join_cmd);
			}
		}
		tmp++;
	}
	//ft_printf("%s", px->cmd_paths[0]);
}

void get_cmds(t_pipex *px, char **args)
{
	char	**split_cmd;
	int		i;

	i = -1;
	px->cmd = ft_calloc(sizeof(char*) , px->nb_cmd + 1);
	if (!px->cmd)
		ft_exit_pipex(px, MALLOC, "get_cmds");
	px->cmd_paths = ft_calloc(sizeof(char*) , px->nb_cmd + 1);
	if (!px->cmd_paths)
		ft_exit_pipex(px, MALLOC, "get_cmds");
	while (++i < px->nb_cmd)
	{
		px->cmd_paths[i] = NULL;
		if (args[i][0] == '\0')
		{
			px->cmd[i] = NULL;
			continue ;
		}
		split_cmd = ft_split(args[i], ' ');
		if (!split_cmd[0])
			ft_exit_pipex(px, MALLOC, "get_cmds");
		px->cmd[i] = ft_strdup(split_cmd[0]);
		if (!px->cmd[i])
			ft_exit_pipex(px, MALLOC, "");
		ft_free_tab(split_cmd);
	}
}

void get_files(t_pipex *px, char **argv, int argc)
{
	px->infile = open(argv[1], O_RDONLY);
	px->outfile = open(argv[argc - 1], O_TRUNC | O_WRONLY | O_CREAT, 0644);
}

void create_child_struct(t_pipex *px)
{
	px->child = malloc(sizeof(t_child) * px->nb_pipes);
	if (!px->child)
		ft_exit_pipex(px, MALLOC, "create_child");
}

void get_cmds_args(t_pipex *px, char **args)
{
	int	i;

	i = -1;
	px->cmd_args = malloc(sizeof(char **) * px->nb_cmd);
	if (!px->cmd_args)
		ft_exit_pipex(px, MALLOC, "get_cmds_args");
	while (++i < px->nb_cmd)
	{
		px->cmd_args[i] = ft_split(args[i], ' ');
		if (!px->cmd_args[i])
			ft_exit_pipex(px, MALLOC, "get_cmds_args");
	}
}

t_pipex *init_struct_values(t_pipex **px, int argc, char **argv, char **envp)
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
	(*px)->nb_pipes = (*px)->nb_cmd - 1;
	create_child_struct((*px));
	get_cmds((*px), args);
	get_cmds_args((*px), args);
	get_files((*px), argv, argc);
	get_full_path(*px, env_full_path);
	get_cmd_paths((*px));
	dprintf(2, "OK");
	return ((*px));
}

void do_in_child(t_pipex *px, int nbr)
{
	close(px->pipes_fd[0]);
	dprintf(1, "child  = 0 :%d\n",px->child[nbr].pid);
	dprintf(1, " child nbr : %d\n",nbr);

//	if (nbr == 0 && px->infile == -1)
//	{
//		perror("nom du fichier: ");
//		exit(0);
//	}
	if (nbr + 1 < px->nb_cmd)
	{
		if (dup2(px->pipes_fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(px->pipes_fd[1]);
	}
	else
	{
		//ft_printf("OUT END FAIL");
		if (dup2(px->outfile, STDOUT_FILENO) == -1)
			exit(0);
		close(px->outfile);
		close(px->pipes_fd[1]);
	}
//	dprintf(1, " nbr cmd : %d\n",px->nb_cmd);
//	dprintf(1, " nbr cmd : %s\n",px->cmd_paths[nbr]);
//	close_fds(2, px->pipes_fd[1], px->infile);
//	dprintf(2,"%s", px->cmd_paths[nbr]);
//	dprintf(2, "%s\n", px->cmd_args[nbr][1]);
	execve(px->cmd_paths[nbr], px->cmd_args[nbr], px->env);
	perror("");
}

void make_child(t_pipex *px, int nbr)
{
	if (nbr > 0)
	{
		if (dup2(px->pipes_fd[0], STDIN_FILENO) == -1)
			exit(1);
		close(px->pipes_fd[0]);
	}
	if (nbr < px->nb_cmd - 1 && pipe(px->pipes_fd) == -1)
		exit(1);
	px->child[nbr].pid = fork();
	if (px->child[nbr].pid == -1)
		exit(1);
	if (px->child[nbr].pid)
		return ;
	// if nbr == 0 check fd input == -1
	// if nbr == last check fd

	if (!px->child[nbr].pid)
		do_in_child(px, nbr);
	//close_fds(2, px->pipes_fd[0], px->pipes_fd[1]);
}

int main(int argc, char **argv, char **envp)
{
	t_pipex *px;
	int i;

	i = -1;
	px = NULL;
	if (argc < 5 || !envp)
		ft_exit_pipex(NULL, ARGC, "");
	px = init_struct_values(&px, argc, argv, envp);
	//make_pipes(px);
	if (dup2(px->infile, STDIN_FILENO) == -1)
	{
		//ft_printf("%d", px->infile);

		printf("%s", strerror(errno));
		i++;
	}
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