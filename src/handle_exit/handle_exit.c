/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 16:45:19 by johnavar          #+#    #+#             */
/*   Updated: 2023/11/17 16:38:57 by jmigoya-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_matrixfree(char ***matrix)
{
	int	i;

	i = 0;
	while (matrix && matrix[0] && matrix[0][i])
	{
		free(matrix[0][i]);
		i++;
	}
	if (matrix)
	{
		free(matrix[0]);
		*matrix = NULL;
	}
}

void	free_scmd(void *content)
{
	t_scmd	*node;

	node = content;
	ft_matrixfree(&node->full_cmd);
	free(node->full_cmd);
	if (node->in_fd != STDIN_FILENO)
		close(node->in_fd);
	if (node->out_fd != STDOUT_FILENO)
		close(node->out_fd);
	free(node);
}

void	handle_exit(t_data *mish)
{
	if (mish)
	{
		if (mish->env)
			hashmap_free_table(mish->env);
		if (mish->cmds)
			ft_lstclear(&mish->cmds, free_scmd);
	}
	exit(g_exit_status);
}

// TODO: the exit code its wrong, maybe we can implement a global with
// the las exit and use here to exit correct.
// TODO: rename function and posibly file name
void	mish_error(t_data *mish, char *param, int err, int is_exit)
{
	if (err == FORK_ERR || err == PIPE_ERR)
		g_exit_status = 1;
	else
		g_exit_status = err;
	if (err != SUCCESS && err != FAILURE && err != ERROR)
	{
		ft_putstr_fd("mish: ", STDERR_FILENO);
		if (err == INV_ARGS)
			ft_putstr_fd("No arguments needed: ", STDERR_FILENO);
		else if (err == FORK_ERR)
			perror("fork");
		else if (err == UNQUOTE)
			ft_putstr_fd("Unmatch quote", STDERR_FILENO);
		else if (err == NO_FILE)
			ft_putstr_fd("No such file or directory: ", STDERR_FILENO);
		else if (err == NO_PERM)
			ft_putstr_fd("Permission denied: ", STDERR_FILENO);
		else if (err == PIPE_ERR)
			ft_putstr_fd("Error creating pipe\n", 2);
		if (param)
			ft_putstr_fd(param, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (is_exit)
		handle_exit(mish);
}
