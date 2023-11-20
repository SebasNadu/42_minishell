/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mish_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmigoya- <jmigoya-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:25:19 by jmigoya-          #+#    #+#             */
/*   Updated: 2023/11/20 16:24:33 by jmigoya-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	mish_pwd(t_data *mish, t_scmd cmd, int if_exit)
{
	printf("pwd init\n");
	char	*str;

	str = hashmap_search(mish->env, "PWD");
	if (str == NULL)
	{
		handle_exit(mish, NULL, FAILURE, if_exit);
	}
	ft_putstr_fd(str, cmd.out_fd);
	ft_putstr_fd("\n", cmd.out_fd);
}
