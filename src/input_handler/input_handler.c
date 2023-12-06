/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:20:01 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/12/06 18:23:26 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Split the input into words(characters between spaces and redirection symbols)
static char	**tokenizer(char *line)
{
	char	**words;
	char	**subwords;
	int		i;

	i = -1;
	words = split_in_words(line, " ");
	while (words && words[++i])
	{
		subwords = split_subwords(words[i], "<|>");
		insert_subwords(&words, subwords, i);
		i += ft_matrixlen(subwords) - 1;
		ft_matrixfree(&subwords);
	}
	return (words);
}

// If there a token after the expansion become into a null for some reason,
// redefine create a new matrix without the null token.
static void	redefine_matrix(char **matrix, int i, int matrix_len)
{
	int		j;
	char	*tmp;

	j = 0;
	while (j < matrix_len)
	{
		if (j < i)
			j++;
		else
		{
			tmp = matrix[j];
			matrix[j] = matrix[j + 1];
			matrix[j + 1] = tmp;
			j++;
		}
	}
}

// Find expander symbols like $ or ~ to expand the variable if it is possible
static void	expander(t_data *mish, char **tokens)
{
	int	i;
	int	quotes[2];
	int	matrix_len;

	matrix_len = ft_matrixlen(tokens);
	i = 0;
	while (tokens && tokens[i])
	{
		tokens[i] = expand_vars(mish, tokens[i], quotes, -1);
		tokens[i] = expand_home(tokens[i], -1, quotes,
				ft_strdup(hashmap_search(mish->env, "HOME")));
		if (!tokens[i])
			redefine_matrix(tokens, i, matrix_len);
		else
			i++;
	}
}

// Trim the quotes of the whole token, if there are quotes.
// Then create a syntatic list, each node has the tokens in a matrix,
// a new node is create if there is a new command and only is possible
// to be a new command if there is a "|" pipe, get_node also define if the token
// represent a redirection or a command, and behaves in relation to it.
static t_list	*fill_syntax_list(t_data *mish, char **tokens, int i)
{
	char	**tmp[2];
	t_list	*cmds[2];

	tmp[0] = trim_all(tokens);
	tmp[1] = tokens;
	cmds[0] = NULL;
	while (tokens[++i])
	{
		cmds[1] = ft_lstlast(cmds[0]);
		if (i == 0 || (tokens[i][0] == '|' && tokens[i + 1]
			&& tokens[i + 1][0]))
		{
			i += tokens[i][0] == '|';
			ft_lstadd_back(&cmds[0], ft_lstnew(init_node()));
			cmds[1] = ft_lstlast(cmds[0]);
		}
		cmds[1]->content = get_node(mish, cmds[1]->content, tmp, &i);
		if (i < 0)
			return (clean_fail(cmds[0], tokens, tmp[0]));
		if (!tokens[i])
			break ;
	}
	ft_matrixfree(&tmp[0]);
	ft_matrixfree(&tokens);
	return (cmds[0]);
}

// Control the parsing of the input. Tokenizer split the input into tokens.
// Expander, handle the expansions. Fill_syntax_list create a list dividing the
// commands and handling the redirections.
void	*input_handler(char *line, t_data *mish)
{
	char	**tokens;

	tokens = tokenizer(line);
	free(line);
	if (!tokens)
	{
		handle_exit(mish, NULL, UNQUOTE, NOT_EXIT);
		return ("");
	}
	expander(mish, tokens);
	mish->cmds = fill_syntax_list(mish, tokens, -1);
	return (mish);
}
