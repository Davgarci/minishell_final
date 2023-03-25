/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 20:41:16 by psegura-          #+#    #+#             */
/*   Updated: 2023/03/25 17:14:04 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*__HEADER FILES__*/
# include "minishell.h"
# include "libs.h"

/*__READLINE______*/
# include "readline.h"

/*__TOKENIZER_____*/
# include "tokenizer.h"

/*__INTRO_________*/
# include "intro.h"

/*__EXECUTOR______*/
# include "executor.h"

/*__BUILTINS______*/
# include "builtins.h"

/*__UTILS_________*/
# include "utils.h"

/*__ERROR_MESSAGES_________*/
# include "error_messages.h"

/*__LIBFT_________*/
# include "../libft/inc/libft.h"

# define READ_END	0
# define WRITE_END	1

/*
	pipe_parse = pipa y antes de redireccion spliteado por espacios
	pipe = 0 si es ultima pipa / 1 si no es la ultima
	read = 1 si pilla imput en redireccion / 0 en el resto / 2 en no sabemos
*/

typedef struct s_pipe
{
	char			**pipe_parse;
	int				pipe;
	int				read;
	int				fd[2];
	struct s_pipe	*next;
	struct s_pipe	*previous;
}					t_pipe;

typedef struct s_cosas {
	char			**env;
	char			**tokens;
	int				tok_count;
	int				flag;
	int				pipas;
	int				dolar_q;
	int				fd[2];
	int				redirection_out;
	int				redirection_in;
	int				read;
	int				forking;
	char			**cmd_exec_parsed;
	int				status_last_command;
	t_pipe			*pipe_list;
}					t_cosas;

extern t_cosas		g_c;
void				ft_lstclear2(t_pipe **lst);

/*___EXECUTOR___*/
void				clear_t_pipe_list2(t_pipe **head);
void				clear_t_pipe_list(t_pipe **head);
void				initialize(int *quotes, int *i, char *quote, int *words);
void				pc_countwords_2(char s, int *quotes, char *quote);
int					pc_countwords(char *s, char c);
int					pc_size_nextword(char *s, char c, int i);
void				pc_save_word2(int *quotes, char *quote, char s);
int					pc_save_word(char *str, char *s, char c, int i);
char				**ft_split_pc(char *s, char c);
t_pipe				*ft_lstlast_pipe(t_pipe *lst);
void				ft_lstadd_back_pipe(t_pipe **alst, t_pipe *newitem);
t_pipe				*new_pipe(char *command, int have_pipe, t_pipe **pipes);
int					ft_redirection_pipe2(char **command);
int					free_else(char ***ret1, char **trimmed1);
int					ft_redirection_pipe(char **command);
void				ft_lstclear2(t_pipe **lst);
int					create_pipes(void);
int					ft_filedescriptors(t_pipe *list_cmnd);
int					ft_child_process(t_pipe *list_cmnd);
void				ft_close_fds(t_pipe *list_cmnd, int pipe_open);
void				ft_pipe2(t_pipe *list_cmnd,
						int *pipe_open, int *ret, pid_t	pid);
int					ft_pipe(t_pipe *list_cmnd, int *pipe_open, int *ret);
int					is_builtin(char *str);
int					only_builtins(t_pipe *head);
void				free_list(t_pipe **head);
void				pipas_handler(void);

#endif
