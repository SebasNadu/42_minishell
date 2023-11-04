/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_testing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migmanu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:21:59 by migmanu           #+#    #+#             */
/*   Updated: 2023/11/04 16:48:20 by migmanu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Initiates user promt with readlie function. Str is an enviroment
// variable that is to be display in the promt, like USER.
void	init_promt(char *str)
{
	char	*symbol;
	char	*var;
	char	*promt;
	char	*line;

	symbol = " $_> ";
	var = getenv(str);
	if (var)
	{
		promt = ft_strjoin(var, symbol);
	}
	else
		promt = symbol;
	while (1)
	{
		line = readline(promt);
		add_history(line);
	}
	free(promt);
}
