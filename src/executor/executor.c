/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmigoya- <jmigoya-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:49:09 by jmigoya-          #+#    #+#             */
/*   Updated: 2023/12/04 14:58:19 by jmigoya-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_cmd(t_data *mish, t_scmd *cmd)
{
	char	**env;

	builtins_router(mish, *cmd, IS_EXIT);
	cmd->path = get_path(mish, cmd->full_cmd[0]);
	hashmap_to_matrix(mish->env, &env, 0, 0);
	if (execve(cmd->path, cmd->full_cmd, env) != 0)
	{
		if (execve(cmd->full_cmd[0], cmd->full_cmd, env) != 0)
		{
			if (cmd->full_cmd[0][0] == '/')
				handle_exit(mish, cmd->full_cmd[0], IS_DIR, IS_EXIT);
			handle_exit(mish, cmd->full_cmd[0], CMD_NOT_FOUND, IS_EXIT);
		}
	}
}

void	fork_cmds(t_data *mish, t_scmd *cmd, int pids[], int c)
{
	pids[c] = fork();
	if (pids[c] == -1)
		handle_exit(mish, NULL, FORK_ERR, NOT_EXIT);
	if (pids[c] == 0)
	{
		dup_cmd(cmd);
		exec_cmd(mish, cmd);
	}
	else
	{
		if (cmd->out_fd != STDOUT_FILENO)
			close(cmd->out_fd);
		if (cmd->in_fd != STDIN_FILENO)
			close(cmd->in_fd);
	}
}

void	executor_loop(t_data *mish, int c)
{
	int		i;
	t_list	*curr;

	curr = mish->cmds;
	i = 0;
	while (curr && i < c)
	{
		fork_cmds(mish, curr->content, mish->pids, i);
		curr = curr->next;
		i++;
	}
}

void	wait_loop(t_data *mish, int c)
{
	int		status;
	int		i;

	i = 0;
	while (i < c - 1)
	{
		waitpid(mish->pids[i], &status, 0);
		i++;
	}
	waitpid(mish->pids[i], &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}

// Handles execution. If only one, built-in command,
// no fork is done.
void	executor(t_data *mish)
{
	t_scmd	*first;
	int		fds[2];
	int		c;

	c = 0;
	if (!mish->cmds)
		return ;
	first = mish->cmds->content;
	if (mish->cmds->next == NULL && check_if_builtin(first->full_cmd[0]) == 0
		&& ft_strncmp("env", first->full_cmd[0], 3) != 0)
	{
		builtins_router(mish, *first, NOT_EXIT);
		return ;
	}
	set_file_descriptors(mish, fds, &c);
	mish->pids = malloc(sizeof(int) * c);
	if (!mish->pids)
		handle_exit(mish, "exec", FAILURE, NOT_EXIT);
	executor_loop(mish, c);
	wait_loop(mish, c);
	if (mish->cmds)
		ft_lstclear(&mish->cmds, free_scmd);
	if (mish->pids)
		free(mish->pids);
}
