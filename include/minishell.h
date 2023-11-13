/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmigoya- <jmigoya-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 16:22:11 by jmigoya-          #+#    #+#             */
/*   Updated: 2023/11/13 17:11:57 by jmigoya-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <linux/limits.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

enum e_mish_err
{
	ERROR = -1,
	SUCCESS,
	FAILURE,
	INV_ARGS,
	FORK_ERR,
	UNQUOTE
};

typedef struct s_hash_item
{
	char				*key;
	char				*value;
	struct s_hash_item	*next;
}			t_hash_item;

typedef struct s_hashmap
{
	t_hash_item		**items;
	unsigned int	size;
	unsigned int	count;
}				t_hashmap;

typedef struct s_scmd
{
	char	**full_cmd;
	char	*path;
	int		in_fd;
	int		out_fd;
}				t_scmd;

typedef struct s_data
{
	t_list		*cmds;
	t_hashmap	*env;
	pid_t		pid;
	char		*pwd;
	char		*old_pwd;
}				t_data;

// hashmap
unsigned long int	hash(char *key, unsigned int size);
t_hash_item			*hashmap_create_item(char *key, char *value);
t_hashmap			*hashmap_create_table(unsigned int size);
void				hashmap_free_item(t_hash_item *item);
void				hashmap_delete(t_hashmap *table, char *key);
void				hashmap_free_table(t_hashmap *table);
void				hashmap_handle_collision(t_hashmap *table, \
							unsigned long int index, t_hash_item *new_item);
void				hashmap_insert(char *key, char *value, t_hashmap *table);
char				*hashmap_search(t_hashmap *table, char *key);
void				hashmap_print_table(t_hashmap *table);

// input_handler.c
void				*input_handler(char *line, t_data *mish);

// exit
void				mish_error(t_data *mish, char *param, int err, int is_exit);
void				ft_free_matrix(char **matrix);

// init
char				*init_prompt(t_data *mish);
void				init_mish(t_data *mish, char *envp[]);
t_hashmap			*env_to_hash(char **env);

// singals.c
void				config_signals(void);

// executor
void				executor_router(t_data *mish);
char				*get_cmd_path(char *cmd, char *env[]);

// testing
void				create_mock_mish(t_data *mish, char *line);

// builtins
int					(*builtins_router(char *str))(t_data *mish, t_scmd cmd);
int					mish_pwd(t_data *mish, t_scmd cmd);
#endif
