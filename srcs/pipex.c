/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurel <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:27:03 by aurel             #+#    #+#             */
/*   Updated: 2023/01/17 16:40:54 by aurel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	get_full_path(t_pipex *px)
{
	char	**tmp;
	char	*env_full_path;
	int 	i;

	i = -1;
	tmp = px->env;
	env_full_path = NULL;
	while (tmp)
	{

		if (ft_strncmp("PATH", *tmp, 4) == 0)
		{
			env_full_path = ft_substr(*tmp, 5, ft_strlen(*tmp));
			break;
		}
		tmp++;
	}
	//ft_printf("%s\n", env_full_path);
	px->env_paths = ft_split(env_full_path, ':');
	//ft_printf("%s\n", px->env_paths[2]);
	tmp = px->env_paths;
	while (tmp[++i])
		px->env_paths[i] = ft_strjoin(tmp[i], "/");
	//px->env_paths[i] = NULL;
//	ft_printf("%s", px->env_paths[0]);
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
				px->cmd_paths[i] = px->cmd[i];
			else if (!px->cmd_paths[i])
			{
				join_cmd = ft_strjoin(*tmp, px->cmd[i]);
				if (!join_cmd)
					exit(1);
				if (access(join_cmd, X_OK) == 0)
					px->cmd_paths[i] = join_cmd;
				else
					free(join_cmd);
			}
			if (i == (px->nb_cmd - 1) && !++tmp && px->cmd_paths[i] == NULL)
				exit(1);
		}
	}
}

void get_cmds(t_pipex *px, char **args)
{
	char	**split_cmd;
	int		i;

	i = -1;
	split_cmd = malloc(sizeof(char*) * px->nb_cmd + 1);
	split_cmd[px->nb_cmd] = "\0";
	px->cmd = malloc(sizeof(char*) * px->nb_cmd + 1);
	px->cmd_paths = malloc(sizeof(char*) * px->nb_cmd + 1);
	px->cmd_paths[px->nb_cmd] = "\0";
	px->cmd[px->nb_cmd] = "\0";
	while (++i < px->nb_cmd)
	{
		px->cmd_paths[i] = NULL;
		split_cmd = ft_split(args[i], ' ');
		if (!split_cmd[0])
			exit(1);
		px->cmd[i] = split_cmd[0];
	}
}

void get_files(t_pipex *px, char **argv, int argc)
{
	px->infile = open(argv[1], O_RDONLY);
	if (px->infile == -1)
		exit(1);
	px->outfile = open(argv[argc - 1], O_TRUNC | O_WRONLY | O_CREAT, 0644);
}

void create_child_struct(t_pipex *px)
{
	int i;

	i = -1;
	px->child = malloc(sizeof(t_child) * px->nb_pipes);
	if (!px->child)
		exit(1);
	while (++i < px->nb_pipes)
		px->child->nbr = i;
}

void get_cmds_args(t_pipex *px, char **args)
{
	int	i;

	i = -1;
	px->cmd_args = malloc(sizeof(char **) * px->nb_cmd + 1);
	while (++i < px->nb_cmd)
	{
		px->cmd_args[i] = ft_split(args[i], ' ');
		if (ft_strlen_tab(px->cmd_args[i]) > 2 || !px->cmd_args[i])
			exit(1);//TODO : free
	}
}

t_pipex *init_struct_values(t_pipex **px, int argc, char **argv, char **envp)
{
	char	**args;
	args = argv + 2;
	*px = malloc(sizeof(t_pipex));
	if (!*px)
		exit(1);
	(*px)->nb_cmd = argc - 3;
	(*px)->env = envp;
	(*px)->nb_pipes = (*px)->nb_cmd - 1;
	create_child_struct((*px));
	get_cmds((*px), args);
	ft_printf("OK");
	get_cmds_args((*px), args);
	get_files((*px), argv, argc);
	get_full_path((*px));
	get_cmd_paths((*px));
	ft_printf("cmd 1 :%s\n", (*px)->cmd_args[0][0]);
	ft_printf("cmd two :%s\n", (*px)->cmd_args[1][0]);
	return ((*px));
}

//void make_pipes(t_pipex *px)
//{
//	int i;
//
//	i = -1;
//	px->pipes_fd = malloc(sizeof(*px->pipes_fd) * px->nb_pipes);
//	if (!px->pipes_fd)
//		exit(1);
//	while (++i < px->nb_pipes)
//	{
//		if (pipe(px->pipes_fd[i]) < 0)
//			exit(1);
//	}
//}

void do_in_child(t_pipex *px, int nbr)
{
	close(px->pipes_fd[0]);
	dprintf(1, "child  = 0 :%d\n",px->child[nbr].pid);
	dprintf(1, " child nbr : %d\n",nbr);
	dprintf(1, " nbr cmd : %d\n",px->nb_cmd);
	if (nbr + 1 < px->nb_cmd && dup2(px->pipes_fd[1], STDOUT_FILENO) == -1)
		exit(1);
	if (nbr + 1 == px->nb_cmd && dup2(px->outfile, STDOUT_FILENO) == -1)
	{
		ft_printf("OUT END FAIL");
		exit(1);
	}
	close_fds(2, px->pipes_fd[1], px->infile);
	execve(px->cmd_paths[nbr], px->cmd_args[nbr], px->env);
	ft_printf("FAIL EXEC");
}

void make_child(t_pipex *px, int nbr)
{
	if (pipe(px->pipes_fd) == -1)
		exit(1);
	px->child[nbr].pid = fork();
	if (px->child[nbr].pid == -1)
		exit(1);
	if (!px->child[nbr].pid)
		do_in_child(px, nbr);
	if (nbr + 1 < px->nb_cmd && dup2(px->pipes_fd[0], STDIN_FILENO) == -1)
		exit(1);
	waitpid(px->child[nbr].pid, NULL, 0);
	close_fds(2, px->pipes_fd[0], px->pipes_fd[1]);
}

int main(int argc, char **argv, char **envp)
{
	t_pipex *px;
	int i;

	i = -1;
	if (argc < 5)
	{
		ft_printf("error argc");
		exit(1);
	}
	px = init_struct_values(&px, argc, argv, envp);
	//make_pipes(px);
	if (dup2(px->infile, STDIN_FILENO) == -1)
	{
		//ft_printf("%d", px->infile);
		//printf("%s", strerror(errno));
		exit(1);
	}
	//execve(px->cmd_paths[1], px->cmd_args[1], px->env);
	while (++i < px->nb_cmd)
		make_child(px, i);
	close_fds(2, px->infile, px->outfile);
	return 0;
}